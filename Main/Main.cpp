#include <iostream>
#include "Scheduler.h"
#include "WeightRoundRobinScheduler.h"
#include "Task.h"
#include "Consts.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"
#include <spdlog/sinks/rotating_file_sink.h>
using namespace std;


void initialize_logger() {
    // Create a daily logger to create a new file every day
    auto daily_logger = spdlog::daily_logger_mt("daily_logger", "logs/daily_log.txt", 0, 0); // Midnight rollover
    spdlog::set_default_logger(daily_logger);
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");

    // Create a rotating file sink to create a new file every hour
    auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        "logs/hourly_log.txt", 1024 * 1024 * 5, 24); // 5 MB size limit, 24 backups

    // Create a logger that uses both daily and hourly sinks
    auto logger = std::make_shared<spdlog::logger>("logger", std::initializer_list<spdlog::sink_ptr>{daily_logger->sinks().back(), rotating_sink});
    spdlog::register_logger(logger);
    spdlog::set_default_logger(logger);

    spdlog::get("logger")->info("Logger initialized and logging to daily and hourly files");

    std::thread([]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(10));
            spdlog::get("logger")->flush();
            spdlog::get("logger")->info("Starting new hourly log file");
        }
        }).detach();
}



int main() {
    initialize_logger();
    Scheduler s;
    s.init();
}
