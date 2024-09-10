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
