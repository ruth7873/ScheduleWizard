#pragma once
#include <string>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/spdlog.h>
#include <spdlog/formatter.h>
#include <spdlog/details/log_msg.h>
#include <sstream>
#include <iomanip>
#include <chrono>
using namespace std;
class HtmlFormatter : public spdlog::formatter {
public:
	void format(const spdlog::details::log_msg& msg, spdlog::memory_buf_t& dest) override;
	std::unique_ptr<spdlog::formatter> clone() const override;
};
class Logger {
	public:
		static void initialize_logger();

		class LoggerInfo {
		public:
			static const string START_SCHEDULER;
			static const string START_THREAD;
			static const string START_EXECUTE;

			static const string CREATE_NEW_TASK;

			static const string TASK_COMPLITED;
			static const string TASK_PREEMPTITVE;
			static const string TASK_SUSPENDED;

			static const string ADD_CRITICAL_TASK;
			static const string ADD_NON_CRITICAL_TASK;
		};

		class LoggerError {
		public:
			static const string ERROR_CREATE_THREAD;
			static const string TASK_TERMINATED;
		};
	};