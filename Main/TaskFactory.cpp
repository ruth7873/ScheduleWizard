#include "TaskFactory.h"

Task TaskFactory::basicInput() {
    std::string priority;
    int runningTime;

    std::cout << "Enter the priority for the task. Options: Critical, Higher, Middle, Lower: \n";
    std::cin >> priority;

    // Input validation for priority
    while (priority != PrioritiesLevel::CRITICAL && priority != PrioritiesLevel::HIGHER &&
        priority != PrioritiesLevel::MIDDLE && priority != PrioritiesLevel::LOWER) {
        spdlog::error("Invalid priority. Please enter one of the specified options.");
        std::cout << "Invalid priority. Please enter one of the specified options." << std::endl;
        std::cout << "Enter the priority for the task. Options: Critical, Higher, Middle, Lower: \n";
        std::cin >> priority;
    }

    // Input validation for runningTime
    runningTime = Utility::validateIntegerInput("Enter the task Running time:", "running Time", 0);
    spdlog::info(Logger::LoggerInfo::CREATE_NEW_TASK, priority, runningTime);

    return Task((Scheduler::taskIds++) % Scheduler::MAX_TASKS, priority, runningTime);
}

std::shared_ptr<Task> TaskFactory::basicTaskInput(bool isOrdered = false) {
    Task basicTask = basicInput();
    return std::make_shared<Task>(basicTask.getId(), basicTask.getPriority(), basicTask.getRunningTime(), isOrdered);
}

shared_ptr<DeadLineTask> TaskFactory::deadLineTaskInput()
{
    Task basicTask = basicInput();
    int dealLineTime = Utility::validateIntegerInput("Enter the Dead line:", "Dead line", 0);

    // Create some DeadlineTask objects
    time_t now = time(nullptr);
    return shared_ptr<DeadLineTask>(new DeadLineTask(basicTask, now + dealLineTime));
}


std::shared_ptr<IterativeTask> TaskFactory::iterativeTaskInput() {
    Task basicTask = basicInput();
    int iterationsRemaining = Utility::validateIntegerInput("Enter the number of repetitions:", "repetition", 0);
    int executionInterval = Utility::validateIntegerInput("Enter the Execution interval between tasks:", "Execution interval", 0);

    return std::make_shared<IterativeTask>(basicTask, iterationsRemaining, executionInterval);
}

std::shared_ptr<Task> TaskFactory::createTask(std::string type) {
    if (type == TaskType::BASIC) {
        return basicTaskInput();
    }
    else if (type == TaskType::ORDERED) {
        return basicTaskInput(true);
    }
    else if (type == TaskType::DEAD_LINE) {
        return std::dynamic_pointer_cast<Task>(deadLineTaskInput());
    }
    else if (type == TaskType::ITERATIVE) {
        return std::dynamic_pointer_cast<Task>(iterativeTaskInput());
    }
    else {
        return nullptr;
    }
}

std::shared_ptr<Task> TaskFactory::createTask(const nlohmann::json& taskData) {
    try {
        std::string taskType = taskData["type"];

        if (taskType == TaskType::BASIC || taskType == TaskType::ORDERED) {
            if (taskData.contains("priority") && taskData.contains("runningTime")) {
                bool isOrdered = taskType == TaskType::ORDERED;
                return std::make_shared<Task>(
                    Scheduler::taskIds++,
                    taskData.at("priority").get<std::string>(),
                    taskData.at("runningTime").get<int>(),
                    isOrdered
                );
            }
            else {
                std::cerr << "Missing required fields for BASIC task!" << std::endl;
            }
        }
        else if (taskType == TaskType::DEAD_LINE) {
            if (taskData.contains("priority") && taskData.contains("runningTime") && taskData.contains("deadLine")) {
                Task basicTask(
                    Scheduler::taskIds++,
                    taskData.at("priority").get<std::string>(),
                    taskData.at("runningTime").get<int>()
                );
                int deadLineTime = taskData.at("deadLine").get<int>();
                auto task = std::make_shared<DeadLineTask>(basicTask, deadLineTime);
                return std::dynamic_pointer_cast<Task>(task);
            }
            else {
                std::cerr << "Missing required fields for DEAD_LINE task!" << std::endl;
            }
        }
        else if (taskType == TaskType::ITERATIVE) {
            if (taskData.contains("priority") && taskData.contains("runningTime") &&
                taskData.contains("iterationsRemaining") && taskData.contains("executionInterval")) {
                Task basicTask(
                    Scheduler::taskIds++,
                    taskData.at("priority").get<std::string>(),
                    taskData.at("runningTime").get<int>()
                );
                int iterationsRemaining = taskData.at("iterationsRemaining").get<int>();
                int executionInterval = taskData.at("executionInterval").get<int>();
                auto task = std::make_shared<IterativeTask>(basicTask, iterationsRemaining, executionInterval);
                return std::dynamic_pointer_cast<Task>(task);
            }
            else {
                std::cerr << "Missing required fields for ITERATIVE task!" << std::endl;
            }
        }
        else {
            std::cout << "Invalid task type!" << std::endl;
            return nullptr;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "An exception occurred: " << e.what() << std::endl;
        spdlog::error("An exception occurred: {}", e.what());
    }

    return nullptr;
}
