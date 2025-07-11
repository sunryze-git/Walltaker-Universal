#include "tray.h"
#include <QApplication>
#include <QIcon>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <filesystem>
#include <iostream>

#include "api.h"
#include "utils.h"
#include "config.h"

TrayManager::TrayManager(const QApplication& app, const ConfigFile& conf) : config(conf) {
    postUrl = QString::fromStdString("https://walltaker.joi.how/api/links/" + config.UserId + "/response.json");
    trayIcon.setIcon(QIcon(":/icon.png")); // Use embedded resource
    trayIcon.setToolTip("Walltaker Universal");

    lastCheckedAction = new QAction("Last checked: Never", &menu);
    lastCheckedAction->setEnabled(false);
    menu.addAction(lastCheckedAction);
    menu.addSeparator();

    openConfigAction = new QAction("Open config.json", &menu);
    QObject::connect(openConfigAction, &QAction::triggered, [] {
        OpenFileWithDefaultApp("./config.json");
    });
    menu.addAction(openConfigAction);

    const std::string tmpDir = std::filesystem::temp_directory_path().string();
    wallpaperPath = QString::fromStdString(tmpDir + "/walltaker_wallpaper.jpg");
    openWallpaperAction = new QAction("Open wallpaper", &menu);
    QObject::connect(openWallpaperAction, &QAction::triggered, [this] {
        OpenFileWithDefaultApp(wallpaperPath.toStdString());
    });
    menu.addAction(openWallpaperAction);
    menu.addSeparator();

    sendResponseA = new QAction("Horny", &menu);
    QObject::connect(sendResponseA, &QAction::triggered, [this] {
        const std::string apiKey = config.ApiKey.empty() ? "" : config.ApiKey;
        std::cerr << "Send Response A triggered\n";
        std::cerr << "Endpoint: " << postUrl.toStdString() << "\n";
        std::cerr << "ApiKey: " << apiKey << "\n";
        if (!API::sendLinkResponse(postUrl.toStdString(), Horny, apiKey.c_str(), "")) {
            showError("Failed to send response.");
        }
    });
    menu.addAction(sendResponseA);

    sendResponseB = new QAction("Disgust", &menu);
    QObject::connect(sendResponseB, &QAction::triggered, [this] {
        const std::string apiKey = config.ApiKey.empty() ? "" : config.ApiKey;
        std::cerr << "Send Response B triggered\n";
        std::cerr << "Endpoint: " << postUrl.toStdString() << "\n";
        std::cerr << "ApiKey: " << apiKey << "\n";
        if (!API::sendLinkResponse(postUrl.toStdString(), Disgust, apiKey.c_str(), "")) {
            showError("Failed to send response.");
        }
    });
    menu.addAction(sendResponseB);

    sendResponseC = new QAction("Came", &menu);
    QObject::connect(sendResponseC, &QAction::triggered, [this] {
        const std::string apiKey = config.ApiKey.empty() ? "" : config.ApiKey;
        std::cerr << "Send Response C triggered\n";
        std::cerr << "Endpoint: " << postUrl.toStdString() << "\n";
        std::cerr << "ApiKey: " << apiKey << "\n";
        if (!API::sendLinkResponse(postUrl.toStdString(), Came, apiKey.c_str(), "")) {
            showError("Failed to send response.");
        }
    });
    menu.addAction(sendResponseC);
    menu.addSeparator();

    quitAction = new QAction("Quit", &menu);
    QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);
    menu.addAction(quitAction);

    trayIcon.setContextMenu(&menu);
    trayIcon.show();
}

void TrayManager::updateLastChecked(const QString& timeStr) const {
    lastCheckedAction->setText("Last checked: " + timeStr);
}

void TrayManager::showError(const QString& errorMsg) {
    trayIcon.showMessage("Walltaker Universal Error", errorMsg, QSystemTrayIcon::Critical);
}

QSystemTrayIcon* TrayManager::getTrayIcon() {
    return &trayIcon;
}

QString TrayManager::getWallpaperPath() const {
    return wallpaperPath;
}
