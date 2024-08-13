#ifndef UTILITIES_H
#define UTILITIES_H

#include <chrono>

bool checkLoopTimeout(const std::chrono::steady_clock::time_point& startTime, int timeoutSeconds);
void initialize_logger();

#endif // UTILITIES_H
