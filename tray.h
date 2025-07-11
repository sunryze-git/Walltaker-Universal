#pragma once
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

#include "config.h"

class TrayManager {
public:
    TrayManager(const QApplication& app, const ConfigFile& conf);
    void updateLastChecked(const QString& timeStr) const;
    void showError(const QString& errorMsg);
    QSystemTrayIcon* getTrayIcon();
    QString getWallpaperPath() const;
private:
    QSystemTrayIcon trayIcon;
    QMenu menu;
    QAction* lastCheckedAction;
    QAction* openConfigAction;
    QAction* openWallpaperAction;
    QAction* sendResponseA;
    QAction* sendResponseB;
    QAction* sendResponseC;
    QAction* quitAction;
    QString wallpaperPath;
    QString postUrl;
    ConfigFile config;
};
