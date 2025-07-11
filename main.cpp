#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <nlohmann/json.hpp>

#include "network.h"
#include "wallpaper.h"
#include "config.h"
#include "utils.h"

#define CHECK_INTERVAL_SECONDS 10

using namespace std;
namespace fs = std::filesystem;
using json = nlohmann::json;

int main() {
    ConfigFile conf;
    if (!LoadConfig(conf, "./config.json")) {
        cerr << "Error loading config.json.";
        return 1;
    }

    IWallpaperSetter* wallpaperSetter = CreateWallpaperSetter();
    INetworkClient* networkClient = CreateNetworkClient();

    const string WALLPAPER_PATH = (fs::temp_directory_path() / "walltaker_wallpaper.jpg").string();
    const string apiUrl = "https://walltaker.joi.how/api/links/" + conf.UserId + ".json";
    while (true) {
        static string lastWallpaperUrl;

        cout << "Checking for new wallpaper...\n";
        string apiResponse = networkClient->FetchUrlContent(apiUrl);
        if (apiResponse.empty()) {
            cerr << "Failed to fetch wallpaper information. Retrying...\n";
            SleepForSeconds(CHECK_INTERVAL_SECONDS);
            continue;
        }

        json parsedJson = json::parse(apiResponse, nullptr, true);
        if (parsedJson.is_discarded() || !parsedJson.contains("post_url")) {
            cerr << "Invalid JSON or missing 'post_url'.\n";
            SleepForSeconds(CHECK_INTERVAL_SECONDS);
            continue;
        }

        auto newWallpaperUrl = parsedJson["post_url"].get<string>();
        if (newWallpaperUrl.empty() || newWallpaperUrl == lastWallpaperUrl) {
            cout << "No new wallpaper detected\n";
            SleepForSeconds(CHECK_INTERVAL_SECONDS);
            continue;
        }

        cout << "New wallpaper URL found: " << newWallpaperUrl << "\n";
        lastWallpaperUrl = newWallpaperUrl;

        cout << "Downloading new wallpaper to: " << WALLPAPER_PATH << "\n";
        if (!networkClient->DownloadFile(newWallpaperUrl, WALLPAPER_PATH)) {
            cerr << "Failed to download wallpaper. Retrying...\n";
            SleepForSeconds(CHECK_INTERVAL_SECONDS);
            continue;
        }

        string absoluteFilePath = fs::canonical(WALLPAPER_PATH).string();
        cout << "Applying wallpaper: " << absoluteFilePath << "\n";
        if (!wallpaperSetter->SetWallpaper(absoluteFilePath)) {
            cerr << "Failed to set wallpaper.\n";
        } else {
            cout << "Wallpaper updated successfully! Waiting for next check...\n";
        }
        SleepForSeconds(CHECK_INTERVAL_SECONDS);
    }
}