#pragma once
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <string>

namespace net = boost::asio;
namespace websocket = boost::beast::websocket;
namespace beast = boost::beast;

// WebSocketSession class handles a single WebSocket connection.
// It is responsible for accepting the connection, reading messages,
// and sending responses back to the client.
class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
public:
    // Constructor: takes ownership of the socket.
    explicit WebSocketSession(net::ip::tcp::socket socket);

    // Initiates the WebSocket session by accepting the handshake.
    void run();

private:
    // Starts asynchronous reading from the WebSocket.
    void do_read();

    // Callback for when a read operation completes.
    void on_read(beast::error_code ec, std::size_t bytes_transferred);

    // Callback for when a write operation completes.
    void on_write(beast::error_code ec, std::size_t bytes_transferred);

    // The WebSocket stream associated with the session.
    websocket::stream<net::ip::tcp::socket> ws_;

    // Buffer for storing incoming messages.
    beast::flat_buffer buffer_;
};
