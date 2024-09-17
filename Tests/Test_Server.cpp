//#include "doctest.h"
//#include "../Main/TCPServer.h"
//#include "../Main/WebSocketSession.h"
//#include "../Main/Scheduler.h"
//#include <boost/asio.hpp>
//#include <memory>
//#include <thread>
//#include <chrono>
//
//using namespace std;
//using namespace boost::asio;
//using namespace boost::asio::ip;
//
//TEST_CASE("Test server starts and accepts connections") {
//
//    Scheduler s(new ReadFromJSON(), new Utility());
//
//    std::thread schedulerThread([&s]() {
//        std::cerr << "Initializing Scheduler...\n";
//        s.init();
//        });
//
//    try {
//        // Initialize the TCP server
//        boost::asio::io_context io_context;
//        TCPServer server(io_context, 8080);  // Port 8080
//
//        // Run the io_context to process incoming tasks
//        io_context.run();
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Exception: " << e.what() << std::endl;
//    }
//    schedulerThread.join();
//}
//
//void runServer(io_context& ioContext, tcp::acceptor& acceptor) {
//    // Example server code to accept connections
//    tcp::socket socket(ioContext);
//    acceptor.accept(socket);
//    // Add more server logic here
//}
//
//TEST_CASE("Test server starts and accepts connections") {
//    io_context ioContext;
//
//    // Set up the server
//    tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), 8080));
//
//    // Start the server in a separate thread
//    std::thread serverThread([&]() {
//        runServer(ioContext, acceptor);
//        });
//
//    // Allow some time for the server to start
//    std::this_thread::sleep_for(std::chrono::milliseconds(10));
//
//    // Simulate a client connection
//    bool connectionSuccessful = false;
//    ioContext.post([&]() {
//        try {
//            tcp::socket socket(ioContext);
//            tcp::endpoint endpoint(tcp::v4(), 8080);
//            socket.connect(endpoint);
//            connectionSuccessful = socket.is_open();
//        }
//        catch (const std::exception& e) {
//            // Log error
//            std::cerr << "Client connection error: " << e.what() << std::endl;
//        }
//        });
//
//    // Allow time for the client connection to be processed
//    std::this_thread::sleep_for(std::chrono::seconds(10));
//
//    CHECK(connectionSuccessful);
//
//    // Clean up
//    ioContext.stop();
//    serverThread.join();
//}
//
////TEST_CASE("Test Server Functionality") {
////    io_context ioContext;  // Boost.Asio I/O context
////
////    // Create a scheduler instance for handling tasks
////    Scheduler scheduler(new ReadFromJSON(), new Utility());
////
////    // Create the server
////    TCPServer server(ioContext, 8080);
////
////    SUBCASE("Test server starts and accepts connections") {
////        // Set up the server
////        tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), 8080));
////        // Start the server in a separate thread
////        std::thread serverThread([&]() {
////            runServer(ioContext, acceptor);
////            });
////
////        // Simulate a client connection
////        ioContext.post([&]() {
////            ip::tcp::socket socket(ioContext);
////            ip::tcp::endpoint endpoint(ip::tcp::v4(), 8080);
////            socket.connect(endpoint);
////
////            // Verify if the server accepted the connection
////            CHECK(socket.is_open());
////            });
////
////        // Give some time for the server to process
////        std::this_thread::sleep_for(std::chrono::seconds(1));
////
////        // Clean up
////        ioContext.stop();
////        serverThread.join();
////    }
//
//    //SUBCASE("Test server processes tasks correctly") {
//    //    // Start the server in a separate thread
//    //    std::thread serverThread([&]() {
//    //        runServer(ioContext, server);
//    //        });
//
//    //    // Simulate a client connection and sending tasks
//    //    ioContext.post([&]() {
//    //        ip::tcp::socket socket(ioContext);
//    //        ip::tcp::endpoint endpoint(ip::tcp::v4(), 8080);
//    //        socket.connect(endpoint);
//
//    //        // Send a task from the client to the server
//    //        std::string taskData = R"({"runtime": 10, "type": "basic", "priority": "Lower"})";  // Example task in JSON format
//    //        write(socket, buffer(taskData));
//
//    //        // Verify if the server processed the task (you may want to check the task insertion in Scheduler)
//    //        CHECK_EQ(scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::LOWER].queue.size(), 1);
//
//    //        });
//
//    //    // Give some time for the server to process
//    //    std::this_thread::sleep_for(std::chrono::seconds(1));
//
//    //    // Clean up
//    //    ioContext.stop();
//    //    serverThread.join();
//    //}
//
//    //SUBCASE("Test server sends correct response to client") {
//    //    // Start the server in a separate thread
//    //    std::thread serverThread([&]() {
//    //        runServer(ioContext, server);
//    //        });
//
//    //    // Simulate a client connection and receiving a response
//    //    ioContext.post([&]() {
//    //        ip::tcp::socket socket(ioContext);
//    //        ip::tcp::endpoint endpoint(ip::tcp::v4(), 8080);
//    //        socket.connect(endpoint);
//
//    //        // Simulate sending a task from the client to the server
//    //        std::string taskData = R"({"runtime": 10, "type": "basic", "priority": "Lower"})";
//    //        write(socket, buffer(taskData));
//
//    //        // Buffer to store server's response
//    //        boost::asio::streambuf responseBuffer;
//    //        read(socket, responseBuffer);
//
//    //        // Convert response to a string and verify it
//    //        std::istream responseStream(&responseBuffer);
//    //        std::string response;
//    //        std::getline(responseStream, response);
//
//    //        CHECK(response == "Task with priority Lower and running time 10 received and scheduled.");  // Expected server response
//    //        });
//
//    //    // Give some time for the server to process
//    //    std::this_thread::sleep_for(std::chrono::seconds(1));
//
//    //    // Clean up
//    //    ioContext.stop();
//    //    serverThread.join();
//    //}
////}
//
//void runServer(io_context& ioContext) {
//    tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), 8080));
//    tcp::socket socket(ioContext);
//    acceptor.accept(socket);
//    // Server logic here - could be handling of connections, etc.
//}
//
//TEST_CASE("Test Server Functionality") {
//    io_context ioContext;  // Boost.Asio I/O context
//
//    // Create a scheduler instance for handling tasks
//    Scheduler scheduler(new ReadFromJSON(), new Utility());
//
//    // Create the server in a separate thread
//    std::thread serverThread([&]() {
//        runServer(ioContext);
//        });
//
//    // Allow the server some time to start up
//    std::this_thread::sleep_for(std::chrono::milliseconds(10));
//
//    SUBCASE("Test server starts and accepts connections") {
//        // Simulate a client connection
//        ip::tcp::socket clientSocket(ioContext);
//        ip::tcp::endpoint endpoint(ip::tcp::v4(), 8080);
//
//        // Connect the client socket to the server
//        clientSocket.connect(endpoint);
//
//        // Verify if the server accepted the connection
//        CHECK(clientSocket.is_open());
//
//        // Clean up client socket
//        clientSocket.close();
//    }
//
//    // Stop the server after the test
//    // You would ideally have some mechanism to safely shutdown your server
//    ioContext.stop();
//    serverThread.join();
//}
//TEST(TCPServerTest, ConnectionHandling) {
//    TCPServer server;
//    ASSERT_TRUE(server.start_accepting()); // Simulate connection
//}
