#include "WebSocketSession.h"
#include <iostream>

// Constructor initializes the WebSocket stream with the given socket.
WebSocketSession::WebSocketSession(net::ip::tcp::socket socket)
    : ws_(std::move(socket)) {}

void WebSocketSession::run() {
    // Accept the WebSocket handshake and start reading messages.
    ws_.accept();
    do_read();
}

void WebSocketSession::do_read() {
    // Asynchronously read a message from the WebSocket.
    ws_.async_read(buffer_,
        [self = shared_from_this()](beast::error_code ec, std::size_t bytes_transferred) {
            self->on_read(ec, bytes_transferred);
        });
}

void WebSocketSession::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    // Handle the read operation result.
    if (ec) {
        if (ec != websocket::error::closed)
            std::cerr << "Read Error: " << ec.message() << std::endl;
        return;
    }

    // Convert the buffer data to a string and print the received message.
    std::string received_message = beast::buffers_to_string(buffer_.data());
    std::cout << "Received message: " << received_message << std::endl;

    // Prepare a response message.
    std::string response = "Task received: " + received_message;

    // Send the response back to the client asynchronously.
    ws_.text(ws_.got_text());
    ws_.async_write(net::buffer(response),
        [self = shared_from_this()](beast::error_code ec, std::size_t bytes_transferred) {
            self->on_write(ec, bytes_transferred);
        });
}

void WebSocketSession::on_write(beast::error_code ec, std::size_t bytes_transferred) {
    // Handle the write operation result and clear the buffer.
    buffer_.consume(buffer_.size());
    do_read();  // Continue reading more messages.
}
