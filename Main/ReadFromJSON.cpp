#include "ReadFromJSON.h"

void ReadFromJSON::createTasksFromJSON(const std::string& filePath) {
    std::ifstream file(filePath);
    json jsonData;

    try {
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filePath << std::endl;
            spdlog::error("Failed to open file: {}", filePath);
            return;
        }

        // Read data from JSON file
        file >> jsonData;

        // Access the "tasks" array in the JSON object
        json tasksData = jsonData["tasks"];

        for (const auto& task : tasksData) {
            // Get the task type from JSON
            std::string taskType = task["type"];

            // Use TaskFactory to create the correct type of task based on the taskType
            std::shared_ptr<Task> newTask = TaskFactory::createTask(task);

            // Insert the new Task into the Scheduler's queues
            if (newTask) {
                Scheduler::insertTask(newTask);
            }
            else {
                std::cerr << "Task creation failed for task type: " << taskType << std::endl;
                spdlog::error("Task creation failed for task type: {}", taskType);
            }

            // If "delay" field exists, wait for the specified delay
            if (task.find("delay") != task.end()) {
                int seconds = task["delay"];
                std::this_thread::sleep_for(std::chrono::milliseconds(seconds));
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "An exception occurred: " << e.what() << std::endl;
        spdlog::error("An exception occurred: {}", e.what());
    }

    file.close();
}
