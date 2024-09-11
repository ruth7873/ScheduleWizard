//
//#include "WebSocketSession.h"
//#include <iostream>
//#include "Task.h"
//#include "Scheduler.h"
//#include <nlohmann/json.hpp>  // For JSON parsing
//
//using json = nlohmann::json;
//
//// Modify the constructor to take a socket and move it into the WebSocket stream
//WebSocketSession::WebSocketSession(boost::asio::ip::tcp::socket socket)
//    : ws_(std::move(socket)) {
//}
//
//boost::asio::ip::tcp::socket& WebSocketSession::get_socket() {
//    return ws_.next_layer();
//}
//
//void WebSocketSession::start() {
//    // Accept the WebSocket handshake
//    ws_.async_accept([self = shared_from_this()](beast::error_code ec) {
//        if (!ec) {
//            self->do_read();
//        }
//        else {
//            std::cerr << "WebSocket accept error: " << ec.message() << std::endl;
//        }
//        });
//}
//
//void WebSocketSession::do_read() {
//    // Read data from the WebSocket
//    ws_.async_read(buffer_, [self = shared_from_this()](beast::error_code ec, std::size_t bytes_transferred) {
//        if (!ec) {
//            // Handle the received message (convert buffer to string)
//            std::string message = beast::buffers_to_string(self->buffer_.data());
//
//            // Log or handle the message as needed
//            std::cout << "Received: " << message << std::endl;
//
//            // Parse the message as JSON
//            json task_json = json::parse(message);
//
//            // Check boundaries before accessing JSON fields
//            if (task_json.contains("priority") && task_json.contains("time")) {
//                std::string priority = task_json["priority"].get<std::string>();
//                std::string time_str = task_json["time"].get<std::string>();
//                int runningTime = std::stoi(time_str);
//
//                // Create a new Task and insert it into the scheduler
//
//                std::shared_ptr<Task> newTask (new Task(Scheduler::taskIds++, priority, runningTime));
//                Scheduler::insertTask(newTask);
//                std::cout << "Sending a response\n";
//
//                // Prepare and send a response to the client
//                std::string response = "Task with priority " + priority + " and running time " +
//                    std::to_string(runningTime) + " received and scheduled.";
//
//                // Use 'self' instead of 'this' to call the member function
//                self->send_response(response);
//            }
//
//            // Clear the buffer
//            self->buffer_.consume(bytes_transferred);
//
//            // Continue reading
//           self->do_read();
//        }
//        else {
//            std::cerr << "WebSocket read error: " << ec.message() << std::endl;
//        }
//        });
//}
//
////void WebSocketSession::send_response(const std::string& response) {
////    // Ensure response is valid
////    if (response.empty()) {
////        std::cerr << "Error: Empty response string" << std::endl;
////        return;
////    }
////
////    ws_.text(ws_.got_text());
////    std::cout << "---- send response in action ----" << std::endl;
////
////    // Capture responseCopy and self
////    ws_.async_write(net::buffer(response),
////        [responseCopy = response, self = shared_from_this()](beast::error_code ec, std::size_t bytes_transferred) {
////            if (ec) {
////                std::cerr << "Write Error: " << ec.message() << " (code: " << ec.value() << ")" << std::endl;
////            }
////            else {
////                std::cout << "Successfully wrote " << bytes_transferred << " bytes" << std::endl;
////            }
////
////            // Continue reading after writing
////            if (self) {
////               // self->do_read(); // Ensure this is implemented correctly
////            }
////            else {
////                std::cerr << "Error: self is no longer valid" << std::endl;
////            }
////        });
////}
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
//    try {
//        // Synchronous write
//        ws_.write(net::buffer(response));
//
//        std::cout << "Successfully wrote " << response.size() << " bytes" << std::endl;
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Write Error: " << e.what() << std::endl;
//    }
//
//    // Continue reading after writing
//    //do_read();
//}
//
//
//void WebSocketSession::do_write(const std::string& message) {
//    // Write data to the WebSocket
//    ws_.async_write(net::buffer(message), [self = shared_from_this()](beast::error_code ec, std::size_t /*bytes_transferred*/) {
//        if (ec) {
//            std::cerr << "WebSocket write error: " << ec.message() << std::endl;
//        }
//        });
//}
