#ifndef WEBSOCKETSESSION_H
#define WEBSOCKETSESSION_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>

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

    // Socket and WebSocket stream
    websocket::stream<boost::asio::ip::tcp::socket> ws_;
    beast::flat_buffer buffer_;
};

#endif
