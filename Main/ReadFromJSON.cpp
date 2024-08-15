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
            Task* newTask = new Task(Scheduler::taskIds++, task["priority"], task["runningTime"], task["status"]);

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
 * @param linesToRead Number of tasks to read at a time.
 * @param delaySeconds Time to wait between reading tasks.
 * @param message Message to display while waiting.
 */
void ReadFromJSON::createTasksFromJSONWithDelay(const string& filePath, int linesToRead, int delaySeconds, string message) {
    try {
        // Read data from JSON file
        std::ifstream file(filePath);
        json jsonData;
        file >> jsonData;

        // Access the "tasks" array in the JSON object
        json tasksData = jsonData["tasks"];

        // Variable to keep track of the number of lines read
        int linesRead = 0;

        // Iterate over the tasks array and create Task objects
        for (const auto& task : tasksData) {
            // Create a new Task object using data from JSON
            Task* newTask = new Task(Scheduler::taskIds++, task["priority"], task["runningTime"]);

            // Insert the new Task into the Scheduler's queues
            Scheduler::insertTask(newTask);

            // Increment the lines read
            linesRead++;

            // Check if the number of lines read equals the specified lines to read
            if (linesRead == linesToRead) {
                // Wait for the specified delay between reading tasks
                auto startTime = std::chrono::steady_clock::now();
                //checkLoopTimeout(startTime, delaySeconds, message);
                std::this_thread::sleep_for(std::chrono::seconds(delaySeconds));  // busy-wait

                // Reset the lines read counter
                linesRead = 0;
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
