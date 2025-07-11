#pragma once
#include <string>

#include "walltaker_opinion.h"
#include "walltaker_response.h"

class API {
public:

    // Fetches the current link object from the server and returns a pointer to a static instance
    static const WalltakerLink* getLinkObject(const std::string& endpoint);

    // Sends a link response to the server (enum type to be defined later)
    static bool sendLinkResponse(const std::string &endpoint, WallpaperOpinion opinion, const std::string &apiKey, const std::string &text);
};