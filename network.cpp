#include "network.h"
#if defined(__linux__)
#include <curl/curl.h>
#include <iostream>
#include <cstdio>

class CurlNetworkClient final : public INetworkClient {
public:
    static size_t WriteDataToString(void* ptr, const size_t size, const size_t nmemb, void* userdata) {
        const auto str = static_cast<std::string*>(userdata);
        str->append(static_cast<char*>(ptr), size * nmemb);
        return size * nmemb;
    }

    std::string FetchUrlContent(const std::string& url) override {
        CURL* curl = curl_easy_init();
        std::string response;
        if (!curl) return "";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteDataToString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_slist* headers = curl_slist_append(nullptr, "User-Agent: walltaker-universal");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        const CURLcode res = curl_easy_perform(curl);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        if (res != CURLE_OK) {
            std::cerr << "CURL error: " << curl_easy_strerror(res) << "\n";
            return "";
        }
        return response;
    }

    bool DownloadFile(const std::string& url, const std::string& outputPath) override {
        CURL* curl = curl_easy_init();
        FILE* file = fopen(outputPath.c_str(), "wb");
        if (!curl || !file) {
            if (file) fclose(file);
            return false;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, nullptr);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        const CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(file);
        if (res != CURLE_OK) {
            std::cerr << "CURL error: " << curl_easy_strerror(res) << "\n";
            return false;
        }
        return true;
    }
};
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <wininet.h>
#include <fstream>
#include <sstream>
#include <iostream>
#pragma comment(lib, "wininet.lib")

class WindowsNetworkClient : public INetworkClient {
public:
    std::string FetchUrlContent(const std::string& url) override {
        HINTERNET hInternet = InternetOpenA("walltaker-universal", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
        if (!hInternet) return "";
        HINTERNET hFile = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 0);
        if (!hFile) {
            InternetCloseHandle(hInternet);
            return "";
        }
        std::stringstream ss;
        char buffer[4096];
        DWORD bytesRead = 0;
        while (InternetReadFile(hFile, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
            ss.write(buffer, bytesRead);
        }
        InternetCloseHandle(hFile);
        InternetCloseHandle(hInternet);
        return ss.str();
    }

    bool DownloadFile(const std::string& url, const std::string& outputPath) override {
        HINTERNET hInternet = InternetOpenA("walltaker-universal", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
        if (!hInternet) return false;
        HINTERNET hFile = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE, 0);
        if (!hFile) {
            InternetCloseHandle(hInternet);
            return false;
        }
        std::ofstream out(outputPath, std::ios::binary);
        char buffer[4096];
        DWORD bytesRead = 0;
        bool success = false;
        while (InternetReadFile(hFile, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
            out.write(buffer, bytesRead);
            success = true;
        }
        out.close();
        InternetCloseHandle(hFile);
        InternetCloseHandle(hInternet);
        return success;
    }
};
#endif

INetworkClient* CreateNetworkClient() {
#if defined(_WIN32) || defined(_WIN64)
    return new WindowsNetworkClient();
#else
    return new CurlNetworkClient();
#endif
}