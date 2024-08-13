#include "Timer.h"
#include <spdlog/spdlog.h>
bool checkLoopTimeout(const std::chrono::steady_clock::time_point& startTime, int timeoutSeconds, std::string message) {
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

    if (elapsed > timeoutSeconds) {
        spdlog::warn("Loop has been running for more than {} seconds.", timeoutSeconds);
        return true;
    }

    return false;
}
