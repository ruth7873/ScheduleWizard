#define _CRT_SECURE_NO_WARNINGS
#include "Logger.h"

void HtmlFormatter::format(const spdlog::details::log_msg& msg, spdlog::memory_buf_t& dest) {
	// Determine color based on log level
	std::string color;
	switch (msg.level) {
	case spdlog::level::info:
		color = "green";
		break;
	case spdlog::level::warn:
		color = "yellow";
		break;
	case spdlog::level::err:
		color = "red";
		break;
	case spdlog::level::debug:
		color = "blue";
		break;
	default:
		color = "black";
		break;
	}

	// Create HTML formatted log message
	std::ostringstream oss;

	// Get current time
	auto now = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);
	std::tm now_tm = *std::localtime(&now_c);

	// Format time and date
	oss << "<p style=\"color:" << color << "\">";
	oss << "[" << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << "] ";
	oss << "[" << spdlog::level::to_string_view(msg.level).data() << "] ";
	oss << msg.payload.data();
	oss << "</p>\n";

	// Append formatted message to destination buffer
	std::string formatted_msg = oss.str();
	dest.append(formatted_msg.data(), formatted_msg.data() + formatted_msg.size());
}

std::unique_ptr<spdlog::formatter> HtmlFormatter::clone() const {
	return std::make_unique<HtmlFormatter>();
}

void Logger::initialize_logger() {
	// Create a daily logger to create a new file every day
	auto daily_logger = spdlog::daily_logger_mt("daily_logger", "logs/daily_log.html", 0, 0); // Midnight rollover

	// Set the custom HTML formatter
	daily_logger->set_formatter(std::make_unique<HtmlFormatter>());

	spdlog::set_default_logger(daily_logger);
	spdlog::set_level(spdlog::level::debug);
	spdlog::get("daily_logger")->debug("Logger initialized and logging to daily and hourly files");

	// Start a new hourly log file periodically
	std::thread([]() {
		while (true) {
			std::this_thread::sleep_for(std::chrono::hours(1)); // Start a new hourly log file every hour
			spdlog::get("daily_logger")->flush();
			spdlog::get("daily_logger")->debug("Starting new hourly log file");
		}
		}).detach();
}

//Logger Info
const string Logger::LoggerInfo::START_SCHEDULER = "Starting scheduling process.";
const string Logger::LoggerInfo::START_THREAD = "{} thread started.";
const string Logger::LoggerInfo::START_READ_FROM_JSON_THREAD = "read tasks From JSON thread started.";
const string Logger::LoggerInfo::START_EXECUTE = "Executing task with ID: {}";

const string Logger::LoggerInfo::CREATE_NEW_TASK = "New task created with priority: {} and running time: {}";

const string Logger::LoggerInfo::TASK_COMPLITED = "Task with ID: {} completed.";
const string Logger::LoggerInfo::TASK_PREEMPTITVE = "Preempting task with ID: {} for real-time task.";
const string Logger::LoggerInfo::TASK_SUSPENDED = "Task with ID: {} suspended and added back to WRR queue.";
const string Logger::LoggerInfo::LONG_TASK_SUSPENDED = "The long task with ID: {} is suspended and will continue later.";

const string Logger::LoggerInfo::ADD_CRITICAL_TASK = "Critical task with ID: {} added to RealTimeScheduler.";
const string Logger::LoggerInfo::ADD_NON_CRITICAL_TASK = "Task with ID: {} added to {} queue.";

const string Logger::LoggerInfo::PUSH_ITERATIVE_TASK_TO_HEAP = "Task with ID: {} added to iterative min heap. return number: {}";
const string Logger::LoggerInfo::POP_ITERATIVE_TASK_FROM_HEAP = "Task with ID: {} popped from iterative min heap.";

//Logger Error
const string Logger::LoggerError::ERROR_CREATE_THREAD = "Error creating threads: {}";
const string Logger::LoggerError::TASK_TERMINATED = "Exception occurred while executing task with ID: {}: {}";
