#include <iostream>
#include "Scheduler.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"
using namespace std;


void initialize_logger() {
    //a daily logger, that exchancge in midnight
    auto daily_logger = spdlog::daily_logger_mt("daily_logger", "logs/daily_log.txt", 19,5);
    spdlog::set_default_logger(daily_logger);
    spdlog::set_level(spdlog::level::info); //the level of the logger is info
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v"); // 
    spdlog::get("daily_logger")->info("Logger initialized and logging to daily file");
}

int main() {
    initialize_logger();
    Scheduler s;
    s.StartScheduling();
    cout << "hellow world";
}
