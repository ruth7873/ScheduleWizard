#pragma once
#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <boost/asio.hpp>
#include <memory>
#include <thread>
#include "Scheduler.h"

// TCPServer class is responsible for setting up a TCP server
// that listens for incoming connections and creates new WebSocket sessions.
class TCPServer {
public:
    // Constructor: initializes the acceptor to listen on the given port.
    TCPServer(boost::asio::io_context& ioc, short port, Scheduler& scheduler);

    // Starts the asynchronous accept loop.
    void start_accepting();

private:
    void do_session(boost::asio::ip::tcp::socket socket);

    boost::asio::ip::tcp::acceptor acceptor_;
    Scheduler& scheduler_;  // Reference to the scheduler
    boost::asio::io_context& ioc_;  // Reference to the I/O context (ADD THIS LINE)

};


#endif
