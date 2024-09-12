#include "ReadFromJSON.h"
#include "TaskFactory.h" 
#include "Consts.h"


/**
 * @brief Reads task data from a JSON file and creates Task objects from it.
 *
 * This function reads task data from a JSON file specified by the filePath parameter.
 * It then creates Task objects based on the data and inserts them into the Scheduler.
 * An optional delay can be added between task reads.
 *
 * @param filePath The path to the JSON file containing task data.
 */
void ReadFromJSON::createTasksFromJSON(const string& filePath) {
	std::ifstream file(filePath);
	json jsonData;

	try {
		// Read data from JSON file
		file >> jsonData;

		// Access the "tasks" array in the JSON object
		json tasksData = jsonData["tasks"];

		// Iterate over the tasks array and create Task objects
		for (const auto& task : tasksData) {
			// Get the task type from JSON
			std::string taskType = task["type"];

			// Use TaskFactory to create the correct type of task based on the taskType
			shared_ptr<Task> newTask = TaskFactory::createTask(task);

			// Insert the new Task into the Scheduler's queues
			if (newTask) {
				Scheduler::insertTask(newTask);
			}
			else {
				std::cerr << "Task creation failed for task type: " << taskType << std::endl;
				spdlog::error(Logger::LoggerError::ERROR_CREATE_TASK, taskType);
			}

			// If delayBetweenTasks is true and a "delay" field exists, wait for the specified delay
			if (task.find("delay") != task.end()) {
				std::this_thread::sleep_for(std::chrono::seconds(task["delay"]));
			}
		}
	}
	catch (const std::exception& e) {
		// Handle exceptions thrown during JSON parsing or task creation
		std::cerr << "An exception occurred: " << e.what() << std::endl;
		spdlog::error(Logger::LoggerError::EXCEPTION_OCCURRED, e.what());
	}

	file.close();
}
