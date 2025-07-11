#pragma once
#include <string>

class INetworkClient {
public:
    virtual ~INetworkClient() = default;
    // Downloads the content at `url` as a string. Returns empty string on failure.
    virtual std::string FetchUrlContent(const std::string& url) = 0;
    // Downloads a file from `url` to `outputPath`. Returns true on success.
    virtual bool DownloadFile(const std::string& url, const std::string& outputPath) = 0;
};

// Factory function to get the correct implementation for the current OS
INetworkClient* CreateNetworkClient();
