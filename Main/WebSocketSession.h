#ifndef WEBSOCKETSESSION_H
#define WEBSOCKETSESSION_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>
#include <iostream>
#include "Task.h"
#include "Scheduler.h"
#include "TaskFactory.h"
#include <nlohmann/json.hpp>  // For JSON parsing
#include <fstream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <mutex>
#include <filesystem> // Use C++17 filesystem
#include <ctime>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
public:
    // Modify the constructor to accept a socket directly
    explicit WebSocketSession(boost::asio::ip::tcp::socket socket);

    // Start the session
    void start();

    // Retrieve the underlying socket
    boost::asio::ip::tcp::socket& get_socket();

private:
    // Function to handle WebSocket read
    void do_read();

    void send_response(const std::string& response);

    // Function to handle WebSocket write
    void do_write(const std::string& message);

    void monitorLogs();

    // Socket and WebSocket stream
    websocket::stream<boost::asio::ip::tcp::socket> ws_;
    beast::flat_buffer buffer_;
};

#endif

std::string get_latest_log_file(const std::string& directory);
