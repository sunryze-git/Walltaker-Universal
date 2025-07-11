#pragma once
#include <string>

struct ConfigFile {
    std::string UserId;
    std::string Username;
    std::string ApiKey;
};

bool LoadConfig(ConfigFile& destConfigStruct, const std::string& filePath);