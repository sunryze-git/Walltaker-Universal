//
// Created by sunryze on 7/11/25.
//

#include "api.h"
#include "network.h"
#include "walltaker_opinion.h"
#include "walltaker_response.h"
#include <nlohmann/json.hpp>
#include <iostream>

const WalltakerLink* API::getLinkObject(const std::string& endpoint) {
    static WalltakerLink currentLink;
    std::string response;
    if (!Network::httpGet(endpoint, response)) {
        std::cerr << "Failed to fetch link object from " << endpoint << std::endl;
        return nullptr;
    }
    try {
        const auto json = nlohmann::json::parse(response);
        currentLink = json.get<WalltakerLink>();
        return &currentLink;
    } catch (const std::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        std::cerr << "Raw response: " << response << std::endl;
        return nullptr;
    }
}

bool API::sendLinkResponse(const std::string& endpoint, const WallpaperOpinion opinion, const std::string& apiKey, const std::string& text) {
    // Convert enum to lowercase string (placeholder logic)
    std::string typeStr;
    switch (opinion) {
        case 0: typeStr = "horny"; break;
        case 1: typeStr = "disgust"; break;
        case 2: typeStr = "came"; break;
        default: typeStr = "unknown"; break; // this should never happen
    }
    nlohmann::json payloadJson;
    payloadJson["api_key"] = apiKey;
    payloadJson["type"] = typeStr;
    payloadJson["text"] = ""; // default to empty string
    if (!text.empty()) {
        payloadJson["text"] = text;
    }
    const std::string payload = payloadJson.dump();
    std::string response;
    const bool success = Network::httpPost(endpoint, payload, response);
    if (!success) {
        std::cerr << "Failed to send link response to " << endpoint << std::endl;
    }
    return success;
}
