//
//#include <iostream>
//#include <nlohmann/json.hpp>  // For JSON parsing
//#include "WebSocketSession.h"
//
//using json = nlohmann::json;
//static int count1 = 0;
//using namespace std;
//
//// Constructor for WebSocketSession
//WebSocketSession::WebSocketSession(net::ip::tcp::socket socket, Scheduler& scheduler)
//    : ws_(std::move(socket)), scheduler_(scheduler) {}
//
//// Run the WebSocket session by accepting the connection
//void WebSocketSession::run() {
//    try {
//        ws_.accept(); // Ensure that the WebSocket connection is accepted
//        do_read();    // Start reading messages
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Error: " << e.what() << std::endl;
//    }
//}
//
//// Asynchronously read a message from the WebSocket
//void WebSocketSession::do_read() {
//    // Clear the buffer before reading
//    buffer_.consume(buffer_.size());
//    std::cout << "---------------1 " << std::endl;
//
//    ws_.async_read(buffer_,
//        [self = shared_from_this()](beast::error_code ec, std::size_t bytes_transferred) {
//            std::cout << "---------------2 " << std::endl;
//
//            if (ec) {
//                std::cerr << "Read Error: " << ec.message() << " (code: " << ec.value() << ")" << std::endl;
//                return; // Exit the read if there's an error
//            }
//
//            if (bytes_transferred > 0) {
//                try {
//                    count1++;
//                    std::cout << "------------- do_read try number calling: " << count1 << std::endl;
//                    self->on_read(ec, bytes_transferred);
//                }
//                catch (const std::exception& e) {
//                    std::cerr << "Exception during on_read: " << e.what() << std::endl;
//                    self->send_error_response("Internal error processing request.");
//                }
//            }
//        });
//}
//
//// Handle the read message from the WebSocket
//void WebSocketSession::on_read(beast::error_code ec, std::size_t bytes_transferred) {
//    if (ec) {
//        if (ec != beast::websocket::error::closed) {
//            std::cerr << "Read Error: " << ec.message() << " (code: " << ec.value() << ")" << std::endl;
//        }
//        return;
//    }
//
//    std::cout << "----------RECEIVED MESSAGE FROM CLIENT---------" << std::endl;
//    cout << "buffer size-----------------" << buffer_.size()<<endl;
//    // Convert the received message from the buffer
//        std::string received_message = beast::buffers_to_string(buffer_.data());
//
//    // Clear the buffer after extracting the string
//    buffer_.consume(buffer_.size());
//
//    // Check if the received message is valid before proceeding
//    if (!received_message.empty()) {
//        std::cout << "Received message: " << received_message << std::endl;
//
//        try {
//            // Parse the message as JSON
//            json task_json = json::parse(received_message);
//
//            // Check boundaries before accessing JSON fields
//            if (task_json.contains("priority") && task_json.contains("time")) {
//                std::string priority = task_json["priority"].get<std::string>();
//                std::string time_str = task_json["time"].get<std::string>();
//                int runningTime = std::stoi(time_str);
//
//                // Create a new Task and insert it into the scheduler
//                Task* newTask = new Task(Scheduler::taskIds++, priority, runningTime);
//                scheduler_.insertTask(newTask);
//                std::cout << "Sending a response\n";
//
//                // Prepare and send a response to the client
//                std::string response = "Task with priority " + priority + " and running time " +
//                    std::to_string(runningTime) + " received and scheduled.";
//                send_response(response);
//            }
//            else {
//                std::cerr << "Invalid JSON format: Missing priority or time." << std::endl;
//                send_error_response("Invalid JSON format: Missing priority or time.");
//            }
//        }
//        catch (const json::parse_error& e) {
//            std::cerr << "JSON Parse Error: " << e.what() << std::endl;
//            send_error_response("Error parsing JSON message.");
//        }
//        catch (const std::exception& e) {
//            std::cerr << "Error: " << e.what() << std::endl;
//            send_error_response("Error processing task.");
//        }
//    }
//    else {
//        std::cerr << "Received an empty message." << std::endl;
//        send_error_response("Received an empty message.");
//    }
//
//    // Continue reading more messages
//    //do_read();
//}
//
////// Send a response back to the client
//void WebSocketSession::send_response(const std::string& response) {
//    ws_.text(ws_.got_text());
//    std::cout << "---- send response in action ----" << std::endl;
//
//    ws_.async_write(net::buffer(response),
//        [responseCopy = response, self = shared_from_this()](beast::error_code ec, std::size_t bytes_transferred) {
//            if (ec) {
//                std::cerr << "Write Error: " << ec.message() << " (code: " << ec.value() << ")" << std::endl;
//            }
//
//            // Clean the buffer after the write operation
//            self->buffer_.consume(self->buffer_.size());  // Ensure the buffer is cleared
//
//            std::cout << "------Continue reading------\n";
//            self->do_read(); // Continue reading after writing
//        });
//}
//
//// Send an error response back to the client
//void WebSocketSession::send_error_response(const std::string& error_message) {
//    ws_.text(ws_.got_text());
//    ws_.async_write(net::buffer(error_message),
//        [self = shared_from_this()](beast::error_code ec, std::size_t bytes_transferred) {
//            if (ec) {
//                std::cerr << "Write Error: " << ec.message() << " (code: " << ec.value() << ")" << std::endl;
//            }
//
//            // Clean the buffer after sending the error response
//            self->buffer_.consume(self->buffer_.size());
//
//            // Continue reading after an error response
//            //self->do_read();
//        });
//}
#include "WebSocketSession.h"
#include <iostream>
#include "Task.h"
#include "Scheduler.h"

#include <nlohmann/json.hpp>  // For JSON parsing

using json = nlohmann::json;

// Modify the constructor to take a socket and move it into the WebSocket stream
WebSocketSession::WebSocketSession(boost::asio::ip::tcp::socket socket)
    : ws_(std::move(socket)) {
}

boost::asio::ip::tcp::socket& WebSocketSession::get_socket() {
    return ws_.next_layer();
}

void WebSocketSession::start() {
    // Accept the WebSocket handshake
    ws_.async_accept([self = shared_from_this()](beast::error_code ec) {
        if (!ec) {
            self->do_read();
        }
        else {
            std::cerr << "WebSocket accept error: " << ec.message() << std::endl;
        }
        });
}

void WebSocketSession::do_read() {
    // Read data from the WebSocket
    ws_.async_read(buffer_, [self = shared_from_this()](beast::error_code ec, std::size_t bytes_transferred) {
        if (!ec) {
            // Handle the received message (convert buffer to string)
            std::string message = beast::buffers_to_string(self->buffer_.data());

            // Log or handle the message as needed
            std::cout << "Received: " << message << std::endl;

            // Parse the message as JSON
            json task_json = json::parse(message);

            // Check boundaries before accessing JSON fields
            if (task_json.contains("priority") && task_json.contains("time")) {
                std::string priority = task_json["priority"].get<std::string>();
                std::string time_str = task_json["time"].get<std::string>();
                int runningTime = std::stoi(time_str);

                // Create a new Task and insert it into the scheduler
                Task* newTask = new Task(Scheduler::taskIds++, priority, runningTime);
                Scheduler::insertTask(newTask);
                std::cout << "Sending a response\n";

                // Prepare and send a response to the client
                std::string response = "Task with priority " + priority + " and running time " +
                    std::to_string(runningTime) + " received and scheduled.";

                // Use 'self' instead of 'this' to call the member function
                self->send_response(response);
            }

            // Clear the buffer
            self->buffer_.consume(bytes_transferred);

            // Continue reading
           self->do_read();
        }
        else {
            std::cerr << "WebSocket read error: " << ec.message() << std::endl;
        }
        });
}

//void WebSocketSession::send_response(const std::string& response) {
//    // Ensure response is valid
//    if (response.empty()) {
//        std::cerr << "Error: Empty response string" << std::endl;
//        return;
//    }
//
//    ws_.text(ws_.got_text());
//    std::cout << "---- send response in action ----" << std::endl;
//
//    // Capture responseCopy and self
//    ws_.async_write(net::buffer(response),
//        [responseCopy = response, self = shared_from_this()](beast::error_code ec, std::size_t bytes_transferred) {
//            if (ec) {
//                std::cerr << "Write Error: " << ec.message() << " (code: " << ec.value() << ")" << std::endl;
//            }
//            else {
//                std::cout << "Successfully wrote " << bytes_transferred << " bytes" << std::endl;
//            }
//
//            // Continue reading after writing
//            if (self) {
//               // self->do_read(); // Ensure this is implemented correctly
//            }
//            else {
//                std::cerr << "Error: self is no longer valid" << std::endl;
//            }
//        });
//}
void WebSocketSession::send_response(const std::string& response) {
    // Ensure response is valid
    if (response.empty()) {
        std::cerr << "Error: Empty response string" << std::endl;
        return;
    }

    ws_.text(ws_.got_text());
    std::cout << "---- send response in action ----" << std::endl;

    try {
        // Synchronous write
        ws_.write(net::buffer(response));

        std::cout << "Successfully wrote " << response.size() << " bytes" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Write Error: " << e.what() << std::endl;
    }

    // Continue reading after writing
    //do_read();
}


void WebSocketSession::do_write(const std::string& message) {
    // Write data to the WebSocket
    ws_.async_write(net::buffer(message), [self = shared_from_this()](beast::error_code ec, std::size_t /*bytes_transferred*/) {
        if (ec) {
            std::cerr << "WebSocket write error: " << ec.message() << std::endl;
        }
        });
}
