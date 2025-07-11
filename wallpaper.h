#pragma once
#include <string>

class IWallpaperSetter {
public:
    virtual ~IWallpaperSetter() = default;
    virtual bool SetWallpaper(const std::string& imagePath) = 0;
};

// Factory function to get the correct implementation for the current OS
IWallpaperSetter* CreateWallpaperSetter();