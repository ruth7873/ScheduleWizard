#include "ReadFromJSON.h"

/**
 * @brief Reads task data from a JSON file and creates Task objects from it.
 *
 * This function reads task data from a JSON file specified by the filePath parameter.
 * It then creates Task objects based on the data and inserts them into the Scheduler.
 *
 * @param filePath The path to the JSON file containing task data.
 */
void ReadFromJSON::createTasksFromJSON(const string& filePath) {
    try {
        // Read data from JSON file
        std::ifstream file(filePath);
        json jsonData;
        file >> jsonData;

        // Access the "tasks" array in the JSON object
        json tasksData = jsonData["tasks"];

        // Iterate over the tasks array and create Task objects
        for (const auto& task : tasksData) {
            // Create a new Task object using data from JSON
            shared_ptr<Task> newTask(new Task(Scheduler::taskIds++, task["priority"], task["runningTime"], task["status"]));

            // Insert the new Task into the Scheduler's queues
            Scheduler::insertTask(newTask);
        }
    }
    catch (const std::exception& e) {
        // Handle exceptions thrown during JSON parsing or task creation
        std::cerr << "An exception occurred: " << e.what() << std::endl;
        // Log the exception
        spdlog::error("An exception occurred: {}", e.what());
    }
}

/**
 * @brief Reads task data from a JSON file and creates Task objects from it with a delay between reads.
 *
 * This function reads task data from a JSON file specified by the filePath parameter.
 * It then creates Task objects based on the data and inserts them into the Scheduler with a specified delay between reads.
 *
 * @param filePath The path to the JSON file containing task data.
 * @param message Message to display while waiting.
 */
void ReadFromJSON::createTasksFromJSONWithDelay(const string& filePath, string message) {
    try {
        // Read data from JSON file
        std::ifstream file(filePath);
        json jsonData;
        file >> jsonData;

        // Access the "tasks" array in the JSON object
        json tasksData = jsonData["tasks"];

        // Iterate over the tasks array and create Task objects
        for (const auto& task : tasksData) {
            // Create a new Task object using data from JSON
            shared_ptr<Task> newTask(new Task(Scheduler::taskIds++, task["priority"], task["runningTime"]));

            // Insert the new Task into the Scheduler's queues
            Scheduler::insertTask(newTask);

            // Check if the "delay" field exists for a task object
            if (task.find("delay") != task.end()) {
                // Wait for the specified delay between reading tasks
                std::this_thread::sleep_for(std::chrono::seconds(task["delay"]));  // busy-wait
            }            
        }
    }
    catch (const std::exception& e) {
        // Handle exceptions thrown during JSON parsing or task creation
        std::cerr << "An exception occurred: " << e.what() << std::endl;
        // Log the exception
        spdlog::error("An exception occurred: {}", e.what());
    }
}
