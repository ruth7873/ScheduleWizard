#include "TCPServer.h"
#include <iostream>
#include "Scheduler.h"
#include <thread>


int main() {
    Scheduler s;
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
