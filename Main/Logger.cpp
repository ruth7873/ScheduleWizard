#include "Logger.h"


void Logger::initialize_logger() {
    // Create a daily logger to create a new file every day
    auto daily_logger = spdlog::daily_logger_mt("daily_logger", "logs/daily_log.txt", 0, 0); // Midnight rollover
    spdlog::set_default_logger(daily_logger);
    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");

    spdlog::get("daily_logger")->info("Logger initialized and logging to daily and hourly files");

    std::thread([]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(10));
            spdlog::get("daily_logger")->flush();
            spdlog::get("daily_logger")->info("Starting new hourly log file");
        }
        }).detach();
}


//Logger Info
const string Logger::LoggerInfo::START_SCHEDULER = "Starting scheduling process.";
const string Logger::LoggerInfo::START_THREAD = "{} thread started.";
const string Logger::LoggerInfo::START_EXECUTE = "Executing task with ID: {}";

const string Logger::LoggerInfo::CREATE_NEW_TASK = "New task created with priority: {} and running time: {}";

const string Logger::LoggerInfo::TASK_COMPLITED = "Task with ID: {} completed.";
const string Logger::LoggerInfo::TASK_PREEMPTITVE = "Preempting task with ID: {} for real-time task.";
const string Logger::LoggerInfo::TASK_SUSPENDED = "Task with ID: {} suspended and added back to WRR queue.";

const string Logger::LoggerInfo::ADD_CRITICAL_TASK = "Critical task with ID: {} added to RealTimeScheduler.";
const string Logger::LoggerInfo::ADD_NON_CRITICAL_TASK = "Task with ID: {} added to {} queue.";

//Logger Error
const string Logger::LoggerError::ERROR_CREATE_THREAD = "Error creating threads: {}";
const string Logger::LoggerError::TASK_TERMINATED = "Exception occurred while executing task with ID: {}: {}";
