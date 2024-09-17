#include "TCPServer.h"
#include <iostream>
#include "Scheduler.h"
#include <thread>


int main() {
    Scheduler s(new ReadFromJSON(), new Utility());
   // s.init();
    std::thread schedulerThread([&s]() {
        std::cerr << "Initializing Scheduler...\n";
        s.init();
        });
    try {
    // Initialize the TCP server
    boost::asio::io_context io_context;
    TCPServer server(io_context, 8080);  // Port 8080
    
    // Run the io_context to process incoming tasks
    io_context.run();
}
catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
}
schedulerThread.join();
return 0;

}

//try {
//    // Create an io_context object to manage asynchronous operations
//    boost::asio::io_context io_context;
//
//    // Create and run the TCPServer instance using std::make_shared
//    auto server = std::make_shared<TCPServer>(io_context, 8080);
//
//    // Run the io_context to start handling events
//    io_context.run();
//}
//
//catch (const std::exception& e) {
//    std::cerr << "Exception: " << e.what() << "\n";
//    return 1;
//}
//schedulerThread.join();
//
//return 0;

