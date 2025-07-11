// utils.cpp
#include "utils.h"
#include <cstdlib>
#include <thread>

std::string GetEnvVar(const std::string& name) {
    const char* val = std::getenv(name.c_str());
    return val ? std::string(val) : std::string();
}

void SleepForSeconds(const int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}