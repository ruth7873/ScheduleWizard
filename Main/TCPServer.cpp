////#include "TCPServer.h"
////#include "WebSocketSession.h"
////#include <iostream>
////
////TCPServer::TCPServer(boost::asio::io_context& ioc, short port, Scheduler& scheduler)
////    : acceptor_(ioc, { boost::asio::ip::tcp::v4(), static_cast<boost::asio::ip::port_type>(port) }),
////    scheduler_(scheduler), ioc_(ioc) {}
////
////void TCPServer::start_accepting() {
////    std::cout << "The server is ready to accept a connection...\n";
////        boost::asio::ip::tcp::socket socket{ ioc_ };
////        acceptor_.accept(socket);
////        std::cout << "Client connected...\n";
////
////        std::make_shared<WebSocketSession>(std::move(socket), scheduler_)->run();
////    
////}
////
////
//#include "TCPServer.h"
//#include <iostream>
//#include "WebSocketSession.h"
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
//            // Create a WebSocket session using the accepted socket
//            auto session = std::make_shared<WebSocketSession>(std::move(*socket));
//            session->start();
//        }
//        else {
//            std::cerr << "Accept error: " << error.message() << std::endl;
//        }
//
//        // Continue accepting new connections
//        start_accept();
//        });
//}
