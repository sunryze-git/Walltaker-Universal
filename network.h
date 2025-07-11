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

class Network {
public:
    // Performs an HTTP GET request to the given URL and stores the response in 'response'. Returns true on success.
    static bool httpGet(const std::string& url, std::string& response);

    // Performs an HTTP POST request to the given URL with the given payload and stores the response in 'response'. Returns true on success.
    static bool httpPost(const std::string& url, const std::string& payload, std::string& response);
};
