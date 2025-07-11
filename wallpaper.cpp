// wallpaper_linux.cpp
#include "wallpaper.h"
#include "utils.h"

#if defined(__linux__)
#include <cstdlib>
#include <string>
#include <iostream>

class LinuxWallpaperSetter final : public IWallpaperSetter {
public:
    bool SetWallpaper(const std::string& imagePath) override {
        const std::string de = DetectDesktopEnvironment();
        std::string command;

        if (de == "KDE") {
            command = "plasma-apply-wallpaperimage -f preserveAspectFit \"" + imagePath + "\"";
        } else if (de == "GNOME") {
            command = "gsettings set org.gnome.desktop.background picture-uri \"file://" + imagePath + "\"";
        } else {
            std::cerr << "Unsupported or unknown desktop environment: " << de << "\n";
            return false;
        }

        const int result = std::system(command.c_str());
        return result == 0;
    }

private:
    static std::string DetectDesktopEnvironment() {
        const std::string xdg_current = GetEnvVar("XDG_CURRENT_DESKTOP");
        const std::string xdg_session = GetEnvVar("XDG_SESSION_DESKTOP");

        if (!xdg_current.empty()) {
            if (xdg_current.find("KDE") != std::string::npos) return "KDE";
            if (xdg_current.find("GNOME") != std::string::npos) return "GNOME";
        }
        if (!xdg_session.empty()) {
            if (xdg_session.find("KDE") != std::string::npos) return "KDE";
            if (xdg_session.find("GNOME") != std::string::npos) return "GNOME";
        }
        return "UNKNOWN";
    }
};
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <string>
#include <iostream>

class WindowsWallpaperSetter final : public IWallpaperSetter {
public:
    bool SetWallpaper(const std::string& imagePath) override {
        // Convert std::string to wide string
        int wchars_num = MultiByteToWideChar(CP_UTF8, 0, imagePath.c_str(), -1, nullptr, 0);
        std::wstring wImagePath(wchars_num, 0);
        MultiByteToWideChar(CP_UTF8, 0, imagePath.c_str(), -1, &wImagePath[0], wchars_num);

        // Set the wallpaper using SystemParametersInfoW
        BOOL result = SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, (PVOID)wImagePath.c_str(), SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE);
        if (!result) {
            std::cerr << "Failed to set wallpaper on Windows. Error: " << GetLastError() << std::endl;
        }
        return result != 0;
    }
};
#endif

IWallpaperSetter* CreateWallpaperSetter() {
#if defined(_WIN32) || defined(_WIN64)
    return new WindowsWallpaperSetter();
#else
    return new LinuxWallpaperSetter();
#endif
}