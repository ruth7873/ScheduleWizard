#include "Logger.h"
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/spdlog.h>
void initialize_logger() {
    // Create a daily logger to create a new file every day
    auto daily_logger = spdlog::daily_logger_mt("daily_logger", "logs/daily_log.txt", 0, 0); // Midnight rollover
    spdlog::set_default_logger(daily_logger);
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");

    spdlog::get("daily_logger")->info("Logger initialized and logging to daily and hourly files");

    std::thread([]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::hours(1));
            spdlog::get("daily_logger")->flush();
            spdlog::get("daily_logger")->info("Starting new hourly log file");
        }
        }).detach();
}
