// utils.cpp
#include "utils.h"
#include <cstdlib>
#include <thread>
#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#endif

std::string GetEnvVar(const std::string& name) {
    const char* val = std::getenv(name.c_str());
    return val ? std::string(val) : std::string();
}

void SleepForSeconds(const int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void OpenFileWithDefaultApp(const std::string& path) {
#ifdef _WIN32
    ShellExecuteA(NULL, "open", path.c_str(), NULL, NULL, SW_SHOWNORMAL);
#elif defined(__APPLE__)
    std::string cmd = "open '" + path + "'";
    system(cmd.c_str());
#else
    std::string cmd = "xdg-open '" + path + "'";
    system(cmd.c_str());
#endif
}
