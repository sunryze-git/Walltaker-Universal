#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <nlohmann/json.hpp>

#include "api.h"
#include "network.h"
#include "wallpaper.h"
#include "config.h"
#include "walltaker_response.h"
#include "tray.h"

#include <QApplication>
#include <QMenu>
#include <QTimer>
#include <QMessageBox>

#define CHECK_INTERVAL_SECONDS 10

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

int main(int argc, char *argv[]) {
    const QApplication app(argc, argv);


    ConfigFile conf;
    switch (LoadConfig(conf, "./config.json")) {
        case 0:
            cout << "Config loaded successfully.\n";
            break;
        case 1:
            QMessageBox::critical(nullptr, "Config Error", "Config file not found, a blank one was created.");
            return 0;
        case 2:
            QMessageBox::critical(nullptr, "Config Error", "Error parsing config file.");
            return 1;
        case 3:
            QMessageBox::critical(nullptr, "Config Error", "Config file is missing some required fields.");
            return 1;
        case 4:
            QMessageBox::critical(nullptr, "Config Error", "Failed to create a template config file. Please report this!");
            return 1;
        default:
            QMessageBox::critical(nullptr, "Config Error", "Unknown error.");
            return 2;
    }
    cout << "Config loaded: UserId = " << conf.UserId << ", ApiKey = " << conf.ApiKey << "\n";

    TrayManager tray(app, conf);
    IWallpaperSetter* wallpaperSetter = CreateWallpaperSetter();
    INetworkClient* networkClient = CreateNetworkClient();

    const string WALLPAPER_PATH = (fs::temp_directory_path() / "walltaker_wallpaper.jpg").string();
    const string apiUrl = "https://walltaker.joi.how/api/links/" + conf.UserId + ".json";
    static string lastWallpaperUrl;

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&] {
        cout << "Checking for new wallpaper...\n";
        const WalltakerLink* link = API::getLinkObject(apiUrl);
        if (!link) {
            tray.showError("Failed to fetch wallpaper information.");
            cerr << "Failed to fetch wallpaper information. Retrying...\n";
            return;
        }
        const auto newWallpaperUrl = link->post_url;
        if (newWallpaperUrl.empty() || newWallpaperUrl == lastWallpaperUrl) {
            cout << "No new wallpaper detected\n";
            return;
        }
        cout << "New wallpaper URL found: " << newWallpaperUrl << "\n";
        lastWallpaperUrl = newWallpaperUrl;
        cout << "Downloading new wallpaper to: " << WALLPAPER_PATH << "\n";
        if (!networkClient->DownloadFile(newWallpaperUrl, WALLPAPER_PATH)) {
            cerr << "Failed to download wallpaper. Retrying...\n";
            return;
        }
        const string absoluteFilePath = fs::canonical(WALLPAPER_PATH).string();
        cout << "Applying wallpaper: " << absoluteFilePath << "\n";
        if (!wallpaperSetter->SetWallpaper(absoluteFilePath)) {
            cerr << "Failed to set wallpaper.\n";
        } else {
            cout << "Wallpaper updated successfully! Waiting for next check...\n";
        }
        // Update last checked time in tray
        const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        char buf[64];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        tray.updateLastChecked(buf);
    });
    timer.start(CHECK_INTERVAL_SECONDS * 1000);
    return QApplication::exec();
}