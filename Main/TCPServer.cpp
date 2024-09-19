#include "TCPServer.h"
#include <iostream>
#include "WebSocketSession.h"


// Constructor to initialize the acceptor and start accepting connections
TCPServer::TCPServer(boost::asio::io_context& io_context, short port)
    : io_context_(io_context),
    acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
    start_accept();
}

void TCPServer::start_accept() {
    // Create a socket for the new connection using the io_context directly
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context_);

    // Start asynchronous accept operation
    acceptor_.async_accept(*socket, [this, socket](const boost::system::error_code& error) {
        if (!error) {
            // Create a WebSocket session using the accepted socket
            auto session = std::make_shared<WebSocketSession>(std::move(*socket));
            session->start();
        }
        else {
            std::cerr << "Accept error: " << error.message() << std::endl;
        }

        // Continue accepting new connections
        start_accept();
        });
}






















//#include "TCPServer.h"
//#include <iostream>
//
//using json = nlohmann::json;
//
//// Initialize the static member outside the class definition
//websocket::stream<boost::asio::ip::tcp::socket> TCPServer::ws_{ boost::asio::ip::tcp::socket(boost::asio::ip::tcp::socket(io_context)) };
//
//
//// Constructor to initialize the acceptor and start accepting connections
//TCPServer::TCPServer(boost::asio::io_context& io_context, short port)
//    : io_context_(io_context),
//    acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
//    start_accept();
//}
//
//void TCPServer::start_accept() {
//    // Create a socket for the new connection using the io_context directly
//    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context_);
//
//    // Start asynchronous accept operation
//    acceptor_.async_accept(*socket, [this, socket](const boost::system::error_code& error) {
//        if (!error) {
//            handle_websocket_session(socket);
//        }
//        else {
//            std::cerr << "Accept error: " << error.message() << std::endl;
//        }
//        start_accept();  // Continue accepting new connections
//        });
//}
//
//void TCPServer::handle_websocket_session(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
//    ws_.next_layer() = std::move(*socket);
//
//    // Accept the WebSocket handshake
//    ws_.async_accept([self = shared_from_this()](beast::error_code ec) {
//        if (!ec) {
//            cout << "Client connected. \n";
//            self->do_read();
//        }
//        else {
//            std::cerr << "WebSocket accept error: " << ec.message() << std::endl;
//        }
//        });
//    {
//
//    }
//}
//
//void TCPServer::do_read() {
//    // Read data from the WebSocket
//    ws_.async_read(buffer_, [self = shared_from_this()](beast::error_code ec, std::size_t bytes_transferred) {
//        if (!ec) {
//            // Handle the received message (convert buffer to string)
//            std::string message = beast::buffers_to_string(self->buffer_.data());
//            std::cout << "Received: " << message << std::endl;
//
//            // Parse the message as JSON
//            json task_json = json::parse(message);
//            if (task_json.contains("priority") && task_json.contains("runningTime")) {
//                // Use TaskFactory to create the correct type of task based on the taskType
//                std::shared_ptr<Task> newTask = TaskFactory::createTask(task_json);
//                if (newTask) {
//                    Scheduler::insertTask(newTask);
//                }
//                else {
//                    std::cerr << "Task creation failed for task type: " << task_json["type"] << std::endl;
//                }
//
//                std::string priority = task_json["priority"].get<std::string>();
//                int runningTime = task_json["runningTime"].get<int>();
//                std::string response = "Task with id: " + std::to_string(newTask->getId()) + " and priority " + priority + " and running time " +
//                    std::to_string(runningTime) + " received and scheduled.";
//                self->send_response(response);
//            }
//
//            self->buffer_.consume(bytes_transferred);
//            self->do_read();
//        }
//        else {
//            std::cerr << "WebSocket read error: " << ec.message() << std::endl;
//        }
//        });
//}
//
//
//void TCPServer::send_response(const std::string& response) {
//	if (response.empty()) {
//		std::cerr << "Error: Empty response string" << std::endl;
//		return;
//	}
//
//	ws_.text(ws_.got_text());
//	std::cout << "---- send response in action ----" << std::endl;
//
//	try {
//		ws_.write(boost::asio::buffer(response));
//		std::cout << "Successfully wrote " << response.size() << " bytes" << std::endl;
//	}
//	catch (const std::exception& e) {
//		std::cerr << "Write Error: " << e.what() << std::endl;
//	}
//}