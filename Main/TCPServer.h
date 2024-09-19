#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <boost/asio.hpp>
#include <memory>

class TCPServer {
public:
    TCPServer(boost::asio::io_context& io_context, short port);

private:
    void start_accept();

    boost::asio::io_context& io_context_;  // Reference to the io_context
    boost::asio::ip::tcp::acceptor acceptor_;
};

#endif // TCPSERVER_H






















//#ifndef TCPSERVER_H
//#define TCPSERVER_H
//
//#include <boost/asio.hpp>
//#include <boost/beast.hpp>
//#include <memory>
//#include <iostream>
//#include "Task.h"
//#include "Scheduler.h"
//#include "TaskFactory.h"
//#include <nlohmann/json.hpp>  // For JSON parsing
//
//namespace beast = boost::beast;
//namespace http = beast::http;
//namespace websocket = beast::websocket;
//namespace net = boost::asio;
//
//class TCPServer : public std::enable_shared_from_this<TCPServer> {
//public:
//    TCPServer(boost::asio::io_context& io_context, short port);
//    static void send_response(const std::string& response);
//    boost::asio::io_context& io_context_;
//
//
//private:
//    // Start accepting incoming connections
//    void start_accept();
//
//    // Handle WebSocket read
//    void do_read();
//
//    // Handle the WebSocket session
//    void handle_websocket_session(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
//
//    // Socket and WebSocket stream
//    static websocket::stream<boost::asio::ip::tcp::socket> ws_ ;
//    beast::flat_buffer buffer_;
//
//    // Accept connections on the TCP server
//    boost::asio::ip::tcp::acceptor acceptor_;
//};
//
////extern TCPServer server; // Declaration of the global server object
//
//#endif // TCPSERVER_H
