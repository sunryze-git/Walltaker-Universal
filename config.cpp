#include "config.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

bool LoadConfig(ConfigFile& destConfigStruct, const std::string& filePath) {
    std::ifstream configFile(filePath);
    if (!configFile) {
        std::cerr << "No config file found at " << filePath << "\n";
        // Create a blank config file with required entries
        nlohmann::json blankConfig = {
            {"Id", ""},
            {"Username", ""},
            {"ApiKey", ""}
        };
        if (std::ofstream newConfigFile(filePath); newConfigFile) {
            newConfigFile << blankConfig.dump(4);
            std::cerr << "Created blank config file at " << filePath << ". Please fill in the required fields.\n";
        } else {
            std::cerr << "Failed to create blank config file at " << filePath << "\n";
        }
        return false;
    }
    nlohmann::json parsedJson;
    try {
        configFile >> parsedJson;
    } catch (const std::exception& e) {
        std::cerr << "Error parsing config: " << e.what() << "\n";
        return false;
    }
    destConfigStruct.UserId = parsedJson.value("Id", "");
    destConfigStruct.Username = parsedJson.value("Username", "");
    destConfigStruct.ApiKey = parsedJson.value("ApiKey", "");
    if (destConfigStruct.UserId.empty() || destConfigStruct.Username.empty() || destConfigStruct.ApiKey.empty()) {
        std::cerr << "Config fields missing or empty.\n";
        return false;
    }
    return true;
}