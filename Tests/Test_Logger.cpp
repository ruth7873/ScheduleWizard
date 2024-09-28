//#define _CRT_SECURE_NO_WARNINGS
//
//#include "doctest.h"
//#include <iostream>
//#include <thread>
//#include <mutex>
//#include <condition_variable>
//#include "spdlog/sinks/daily_file_sink.h"
//#include "../Main/Logger.h"
//
//// Helper function to get the formatted output
//std::string get_log_contents(const std::string& file_path) {
//    std::ifstream file(file_path);
//    if (!file.is_open()) {
//        return "";
//    }
//    std::stringstream buffer;
//    buffer << file.rdbuf();
//
//    if (buffer.str().empty()) {
//        std::cerr << "Buffer is empty, file may not contain any content or read operation failed." << std::endl;
//    }
//
//    return buffer.str();
//}
//
//std::string get_current_date_string() {
//    std::time_t now = std::time(nullptr);
//    std::tm* local_tm = std::localtime(&now);
//    std::stringstream date_stream;
//    date_stream << std::put_time(local_tm, "%Y-%m-%d"); // Example format: 2024-09-08
//    return date_stream.str();
//}
//TEST_CASE("get_log_contents handles file open failure") {
//    // Attempt to read from a non-existent file
//    std::string non_existent_file = "non_existent_file.txt";
//    std::string result = get_log_contents(non_existent_file);
//
//    // Check that the function returns an empty string when the file can't be opened
//    CHECK(result.empty());
//}
//TEST_CASE("Logger Initialization and Formatting") {
//    // Initialize logger
//    Logger::initialize_logger();
//
//    // Test logger initialization
//    CHECK_NOTHROW(spdlog::get("daily_logger")->debug("Test message"));
//
//    std::string date_string = get_current_date_string();
//    std::string file_path = "logs/daily_log_" + date_string + ".html";
//
//    std::this_thread::sleep_for(std::chrono::milliseconds(11));//the content is writen once a 10 seconds
//
//    // Check if the log file has been created and has the correct content
//    const std::string log_content = get_log_contents(file_path);
//
//    // Print out the log content for debugging
//    std::cout << "Log Content:\n" << log_content << std::endl;
//
//    SUBCASE("Test log message formatting") {
//        // Ensure the log contains a message with the correct HTML structure
//        CHECK(log_content.find("<p style=\"color:blue\">") != std::string::npos);  // Debug message should be in blue
//        CHECK(log_content.find("Test message") != std::string::npos);
//    }
//}
//
//TEST_CASE("HtmlFormatter: Formatting Log Messages") {
//    HtmlFormatter formatter;
//    spdlog::memory_buf_t buffer;
//
//    spdlog::details::log_msg msg;
//    SUBCASE("Formatting Info Message") {
//        msg.level = spdlog::level::info;
//        msg.payload = spdlog::string_view_t("This is an info message");
//
//        formatter.format(msg, buffer);
//
//        // Check that the formatted message contains the expected HTML tags
//        std::string result(buffer.data(), buffer.size());
//        CHECK(result.find("<p style=\"color:green\">") != std::string::npos);  // Info message should be in green
//        CHECK(result.find("This is an info message") != std::string::npos);
//    }
//    SUBCASE("Formatting Warning Message") {
//        msg.level = spdlog::level::warn;
//        msg.payload = spdlog::string_view_t("This is a warning message");
//
//        formatter.format(msg, buffer);
//
//        // Check that the formatted message contains the expected HTML tags
//        std::string result(buffer.data(), buffer.size());
//        CHECK(result.find("<p style=\"color:yellow\">") != std::string::npos);  // Warning message should be in yellow
//        CHECK(result.find("This is a warning message") != std::string::npos);
//    }
//    SUBCASE("Formatting Error Message") {
//        msg.level = spdlog::level::err;
//        msg.payload = spdlog::string_view_t("This is an error message");
//
//        formatter.format(msg, buffer);
//
//        // Check that the formatted message contains the expected HTML tags
//        std::string result(buffer.data(), buffer.size());
//        CHECK(result.find("<p style=\"color:red\">") != std::string::npos);  // Error message should be in red
//        CHECK(result.find("This is an error message") != std::string::npos);
//    }
//    SUBCASE("Formatting a Default Message") {
//        msg.level = spdlog::level::off;
//        msg.payload = spdlog::string_view_t("This is a default message");
//
//        formatter.format(msg, buffer);
//
//        // Check that the formatted message contains the expected HTML tags
//        std::string result(buffer.data(), buffer.size());
//        CHECK(result.find("<p style=\"color:black\">") != std::string::npos);  // Default message should be in black
//        CHECK(result.find("This is a default message") != std::string::npos);
//    }
//}
//TEST_CASE("HtmlFormatter: Clone Function") {
//    HtmlFormatter original_formatter;
//
//    // Clone the formatter
//    auto cloned_formatter = original_formatter.clone();
//
//    // Check that the cloned formatter is not null
//    CHECK(cloned_formatter != nullptr);
//
//    // Check that the cloned formatter is a different object
//    CHECK(&original_formatter != cloned_formatter.get());
//
//    // Test that the cloned formatter behaves the same as the original
//    spdlog::memory_buf_t original_buffer, cloned_buffer;
//    spdlog::details::log_msg msg;
//    msg.level = spdlog::level::
// ;
//    msg.payload = spdlog::string_view_t("Test message");
//
//    original_formatter.format(msg, original_buffer);
//    cloned_formatter->format(msg, cloned_buffer);
//
//    std::string original_result(original_buffer.data(), original_buffer.size());
//    std::string cloned_result(cloned_buffer.data(), cloned_buffer.size());
//
//    // Check that both formatters produce the same output
//    CHECK(original_result == cloned_result);
//}
