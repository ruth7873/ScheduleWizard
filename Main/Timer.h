#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <string>

using namespace std;

bool checkLoopTimeout(const std::chrono::steady_clock::time_point& startTime, int timeoutSeconds, string message);

#endif // TIMER_H

