//#include "Timer.h"
//bool checkLoopTimeout(const std::chrono::steady_clock::time_point& startTime, int timeoutSeconds, std::string message) {
//    auto currentTime = std::chrono::steady_clock::now();
//    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();//Counts how many seconds have passed from the starting time to now
//
//    if (elapsed > timeoutSeconds) {
//        spdlog::warn("Loop has been running for more than {} seconds.", timeoutSeconds);
//        spdlog::warn(message);
//
//        return true;
//    }
//    return false;
//}