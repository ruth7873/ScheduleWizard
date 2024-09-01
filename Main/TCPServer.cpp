#include "TCPServer.h"
#include <iostream>

TCPServer::TCPServer(boost::asio::io_context& ioc, short port, Scheduler& scheduler)
    : acceptor_(ioc, { boost::asio::ip::tcp::v4(), static_cast<boost::asio::ip::port_type>(port) }),
    scheduler_(scheduler) ,  // Initialize scheduler reference
    ioc_(ioc) {}  // Initialize scheduler reference and store ioc

void TCPServer::start_accepting() {
    for (;;) {
        //boost::asio::ip::tcp::socket socket{ acceptor_.get_executor().context() };
        boost::asio::ip::tcp::socket socket{ ioc_ };  // Use the stored I/O context directly
        acceptor_.accept(socket);
        std::thread([this, s = std::move(socket)]() mutable {
            this->do_session(std::move(s));
            }).detach();
    }
}

void TCPServer::do_session(boost::asio::ip::tcp::socket socket) {
    try {
        boost::asio::ip::tcp::endpoint remote_ep = socket.remote_endpoint();
        std::string remote_address = remote_ep.address().to_string();

        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, "\n");
        std::istream is(&buffer);
        std::string message;
        std::getline(is, message);

        std::cout << "Received message from " << remote_address << ": " << message << std::endl;
        std::cerr << "Received message from " << remote_address << ": " << message << std::endl;

        // Parse the message and insert a task
        Task* newTask = scheduler_.input();  // This assumes you parse the message into a Task
        scheduler_.insertTask(newTask);

        boost::asio::write(socket, boost::asio::buffer("Task received\n"));

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
