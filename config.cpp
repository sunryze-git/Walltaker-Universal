#include "config.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

// Return codes:
// 0: Success
// 1: Config file not found, created a blank one.
// 2: Error parsing config file.
// 3: Config file missing required fields.
// 4: Failed to create blank config file.
int LoadConfig(ConfigFile& destConfigStruct, const std::string& filePath) {
    std::ifstream configFile(filePath);
    if (!configFile) {
        nlohmann::json blankConfig = {
            {"Id", ""},
            {"Username", ""},
            {"ApiKey", ""}
        };
        if (std::ofstream newConfigFile(filePath); newConfigFile) {
            newConfigFile << blankConfig.dump(4);
            return 1; // Config file not found, created a blank one.
        }
        return 4; // Failed to create blank config file.
    }
    nlohmann::json parsedJson;
    try {
        configFile >> parsedJson;
    } catch (const std::exception& e) {
        return 2; // Error parsing config file.
    }
    destConfigStruct.UserId = parsedJson.value("Id", "");
    destConfigStruct.Username = parsedJson.value("Username", "");
    destConfigStruct.ApiKey = parsedJson.value("ApiKey", "");
    if (destConfigStruct.UserId.empty() || destConfigStruct.Username.empty() || destConfigStruct.ApiKey.empty()) {
        return 3; // Config file missing required fields.
    }
    return 0; // Success
}