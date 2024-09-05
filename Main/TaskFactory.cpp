#include "TaskFactory.h"


/**
 * @brief This function allows the user to input details for a new task, including priority and running time.
 *
 * @return A new created Task object based on the user input.
 */
Task TaskFactory::basicInput()
{
		std::string priority;
		int runningTime;

		std::cout << "Enter the priority for the task. Options: Critical, Higher, Middle, Lower: \n";
		std::cin >> priority;

		// Input validation for priority
		while (priority != PrioritiesLevel::CRITICAL && priority != PrioritiesLevel::HIGHER &&
			priority != PrioritiesLevel::MIDDLE && priority != PrioritiesLevel::LOWER) {
			spdlog::error("Invalid priority. Please enter one of the specified options.");
			std::cout << "Invalid priority. Please enter one of the specified options." << std::endl;
			std::cout << "Enter the priority for the task. Options: Critical, Higher, Middle, Lower: ";
			std::cin >> priority;
		}

		// Input validation for runningTime
		runningTime = Utility::integerValidation("Enter the task Running time:", "running Time", 0);
		spdlog::info(Logger::LoggerInfo::CREATE_NEW_TASK, priority, runningTime);

		return Task((Scheduler::taskIds++) % Scheduler::MAX_TASKS, priority, runningTime);
	}

shared_ptr<Task> TaskFactory::basicTaskInput()
{
	Task basicTask = basicInput();
    return shared_ptr<Task>(new Task(basicTask.getId(), basicTask.getPriority(), basicTask.getRunningTime()));
}

shared_ptr<DeadLineTask> TaskFactory::deadLineTaskInput()
{
	Task basicTask = basicInput();
	int dealLineTime = Utility::integerValidation("Enter the Dead line:", "Dead line", 0);
	return shared_ptr<DeadLineTask>(new DeadLineTask(basicTask, dealLineTime));
}

shared_ptr<IterativeTask> TaskFactory::iterativeTaskInput()
{
	Task basicTask = basicInput();
	int iterationsRemaining = Utility::integerValidation("Enter the number of repetitions:", "repetition", 0);
	int executionInterval = Utility::integerValidation("Enter the Execution interval between tasks:", "Execution interval", 0);
	return shared_ptr<IterativeTask>(new IterativeTask(basicTask, iterationsRemaining, executionInterval));
}

shared_ptr<Task> TaskFactory::createTask(string type)
{
    if (type == TaskType::BASIC) {
		cout << "enter to build basic" << endl;
        return basicTaskInput();
    }
    else if (type == TaskType::DEAD_LINE) {
        return deadLineTaskInput();
    }
    else if (type == TaskType::ITERATIVE) {
        return iterativeTaskInput();
    }
	else{
		cout << "i dont need to print this!!" << endl;
		return nullptr;
	}
}
