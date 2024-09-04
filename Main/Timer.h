#pragma once
#include <chrono>
#include <string>
#include <spdlog/spdlog.h>

using namespace std;

bool checkLoopTimeout(const std::chrono::steady_clock::time_point& startTime, int timeoutSeconds, string message);