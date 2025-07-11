// utils.h
#pragma once
#include <string>

// Returns the value of the environment variable, or empty string if not set.
std::string GetEnvVar(const std::string& name);

// Sleeps for the given number of seconds (platform-independent).
void SleepForSeconds(int seconds);