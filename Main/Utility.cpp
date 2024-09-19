#include "Utility.h"
#include "Scheduler.h"

int Utility::validateIntegerInput(const std::string& inputMessage, const std::string& variableName, bool allowNegative) {
    int numberInput;
    std::string input;

    while (true) {
        std::cout << inputMessage << std::endl;
        std::cin >> input;

        // Check if the input contains only digits
        if (input.find_first_not_of("0123456789-") != std::string::npos) {
            std::cerr << "Invalid input. " << variableName << " should contain only numeric digits." << std::endl;
            continue; // Restart the loop for a valid input
        }

        // Convert the input to an integer
        numberInput = std::stoi(input);

        if (!allowNegative && numberInput < 0) {
            std::cerr << "Invalid input: " << variableName << " cannot be negative." << std::endl;
            continue; // Restart the loop for a valid input
        }

        break; // If numberInput is valid, exit the loop
    }

    return numberInput;
}

void Utility::displayInviteMessage(Task* task) {
    if (task->getStatus() != TaskStatus::CREATION)
        Scheduler::displayMessage(task);
}

void Utility::checkTaskIds()
{
    if (Scheduler::taskIds == Scheduler::MAX_TASKS * 0.8)
        spdlog::warn("System overload, the number of tasks has exceeded 80% of the capacity");

    if (Scheduler::taskIds == Scheduler::MAX_TASKS)
        spdlog::error("The number of tasks in the system has exceeded its capacity. Please contact the manufacturer for a fix.");
}


void Utility::insertTaskFromInput() {
    TaskFactory taskFactory;

    while (true) {
        std::cout << "Enter task type: basic/deadLine/iterative/ordered:" << std::endl;
        std::string type;
        std::cin >> type;

        // Validate the input task type
        if (type == TaskType::BASIC || type == TaskType::DEAD_LINE || type == TaskType::ITERATIVE || type == TaskType::ORDERED) {
            std::shared_ptr<Task> newTask = taskFactory.createTask(type);
            Scheduler::insertTask(newTask);
        }
        else {
            std::cout << "Invalid task type." << std::endl;
        }
    }
}
