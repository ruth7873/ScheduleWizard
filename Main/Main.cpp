//
//#include "TCPServer.h"
//#include <boost/asio.hpp>
//#include <iostream>
//#include "Scheduler.h"
//#include <thread>
//
//int main() {
//    Scheduler s;
//    std::thread schedulerThread([&s]() {
//        std::cerr << "Initializing Scheduler...\n";
//        s.init();  // Run the scheduler in its own thread
//        });
//
//    try {
//        std::cerr << "Starting server...\n";
//        boost::asio::io_context ioc{ 1 };  // Create I/O context with one thread.
//        TCPServer server(ioc, 8080, s);    // Initialize the server with the scheduler.
//
//        std::cerr << "The server started listening\n";  // Print before starting the accept loop
//        std::thread serverThread([&server]() {
//            server.start_accepting();  // Start accepting incoming connections in a separate thread
//            });
//
//        ioc.run();  // Run the I/O service to process events
//
//        serverThread.join();  // Ensure the server thread is joined before exiting
//    }
//    catch (const std::exception& e) {
//        // Handle any exceptions that occur during execution.
//        std::cerr << "Error: " << e.what() << std::endl;
//        return EXIT_FAILURE;
//    }
//
//    schedulerThread.join();  // Ensure the scheduler thread is joined before exiting
//    return EXIT_SUCCESS;     // Indicate successful execution.
//}
