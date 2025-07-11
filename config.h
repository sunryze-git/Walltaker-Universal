#pragma once
#include <string>

struct ConfigFile {
    std::string UserId;
    std::string Username;
    std::string ApiKey;
};

int LoadConfig(ConfigFile& destConfigStruct, const std::string& filePath);