#include "WebSocketSession.h"
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING


using json = nlohmann::json;
namespace fs = std::filesystem; // Use standard filesystem instead of experimental

// Path to the logs directory
const std::string logDirectoryPath = ".\\logs";

// Modify the constructor to take a socket and move it into the WebSocket stream
WebSocketSession::WebSocketSession(boost::asio::ip::tcp::socket socket)
	: ws_(std::move(socket)) {

	// Start the log monitoring thread
	std::thread(&WebSocketSession::monitorLogs, this).detach();

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
			if (task_json.contains("priority") && task_json.contains("runningTime")) {
				// Use TaskFactory to create the correct type of task based on the taskType
				shared_ptr<Task> newTask = TaskFactory::createTask(task_json);

				// Insert the new Task into the Scheduler's queues
				if (newTask) {
					Scheduler::insertTask(newTask);
				}
				else {
					std::cerr << "Task creation failed for task type: " << task_json["type"] << std::endl;
					//
					// 
					// 
					// 
					// ::error("Task creation failed for task type: {}", task_json["type"]);
				}
				// Check boundaries before accessing JSON fields

				std::string priority = task_json["priority"].get<std::string>();
				int runningTime = task_json["runningTime"].get<int>();  // Expecting an integer

				// Create a new Task and insert it into the scheduler

				//std::shared_ptr<Task> newTask(new Task(Scheduler::taskIds++, priority, runningTime));
				//Scheduler::insertTask(newTask);
				std::cout << "Sending a response\n";

				// Prepare and send a response to the client
				std::string response = "Task with ID: "+std::to_string(newTask->getId()) + "  priority " + priority + " and running time " +
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


// Helper function to get the latest log file
std::string WebSocketSession::get_latest_log_file(const std::string& directory) {
	std::string latestFile;
	std::time_t latestTime = 0;

	for (const auto& entry : fs::directory_iterator(directory)) {
		if (fs::is_regular_file(entry)) {
			// Get the last write time of the file
			auto ftime = fs::last_write_time(entry);

			// Convert file_time_type to system_clock::time_point
			auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
				ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
			);

			// Convert to time_t
			std::time_t fileTime_t = std::chrono::system_clock::to_time_t(sctp);

			// Check if this file is the latest one
			if (fileTime_t > latestTime) {
				latestTime = fileTime_t;
				latestFile = entry.path().string();
			}
		}
	}

	return latestFile;
}

void WebSocketSession::monitorLogs() {
	std::string logFilePath = get_latest_log_file(logDirectoryPath);
	std::ifstream logFile(logFilePath);

	// Track the position in the log file
	std::streampos lastPosition = logFile.tellg();

	while (true) {
		{
			// Open log file and check for new entries
			if (!logFile.is_open()) {
				logFile.open(logFilePath);
				if (!logFile.is_open()) {
					std::cerr << "Error opening log file: " << logFilePath << std::endl;
					std::this_thread::sleep_for(std::chrono::seconds(1));
					continue;
				}
			}

			// Move the file pointer to the last known position
			logFile.seekg(lastPosition);

			std::string line;
			bool newLinesRead = false;

			// Read new lines from the file
			while (std::getline(logFile, line)) {
				newLinesRead = true;

				// Process the line and send response if necessary (as in your existing code)
				size_t start = line.find("<p");
				size_t end = line.find("</p>");
				if (start != std::string::npos && end != std::string::npos) {
					size_t contentStart = line.find(">", start) + 1;
					std::string trimmedLine = line.substr(contentStart, end - contentStart);
					if (trimmedLine.find("Executing") != std::string::npos ||
						trimmedLine.find("completed") != std::string::npos ||
						trimmedLine.find("suspended") != std::string::npos) {
						std::cout << " <<<< Reading from the log file: " << trimmedLine << std::endl;
						send_response(trimmedLine);
					}
				}

				// Update the last known position
				lastPosition = logFile.tellg();
			}

			// Close the log file after reading
			logFile.close();

			// Handle log rotation if needed (as in your existing code)
			if (!newLinesRead) {
				std::string latestLogFilePath = get_latest_log_file(logDirectoryPath);
				if (latestLogFilePath != logFilePath) {
					logFile.close();
					logFilePath = latestLogFilePath;
					logFile.open(logFilePath);
					if (!logFile.is_open()) {
						std::cerr << "Error opening new log file: " << logFilePath << std::endl;
						std::this_thread::sleep_for(std::chrono::seconds(1));
						continue;
					}
					lastPosition = logFile.tellg();
				}
			}
		}
		// Sleep briefly before checking for new entries again
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
