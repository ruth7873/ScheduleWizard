#include "TaskFactory.h"


using namespace std;

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
		std::cout << "Enter the priority for the task. Options: Critical, Higher, Middle, Lower: \n";
		std::cin >> priority;
	}

	// Input validation for runningTime
	runningTime = Utility::integerValidation("Enter the task Running time:", "running Time", 0);
	spdlog::info(Logger::LoggerInfo::CREATE_NEW_TASK, priority, runningTime);

	return Task((Scheduler::taskIds++) % Scheduler::MAX_TASKS, priority, runningTime);
}

shared_ptr<Task> TaskFactory::basicTaskInput(bool isOrdered = false)
{
	Task basicTask = basicInput();
	return shared_ptr<Task>(new Task(basicTask.getId(), basicTask.getPriority(), basicTask.getRunningTime(), isOrdered));
}

shared_ptr<DeadLineTask> TaskFactory::deadLineTaskInput()
{
	Task basicTask = basicInput();
	int dealLineTime = Utility::integerValidation("Enter the Dead line:", "Dead line", 0);

	// Create some DeadlineTask objects
	time_t now = time(nullptr);
	return shared_ptr<DeadLineTask>(new DeadLineTask(basicTask, now + dealLineTime));
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
		return basicTaskInput();
	}
	else if (type == TaskType::ORDERED)
		return basicTaskInput(true);
	else if (type == TaskType::DEAD_LINE) {
		return dynamic_pointer_cast<Task>(deadLineTaskInput());
	}
	else if (type == TaskType::ITERATIVE) {
		return dynamic_pointer_cast<Task>(iterativeTaskInput());
	}
	else {
		return nullptr;
	}
}

shared_ptr<Task> TaskFactory::createTask(const nlohmann::json& taskData)
{
	try {
		std::string taskType = taskData["type"];

		// Check if taskData contains the required keys for a basic task
		if (taskType == TaskType::BASIC || taskType == TaskType::ORDERED) {
			if (taskData.contains("priority") && taskData.contains("runningTime")) {
				auto priority = taskData.at("priority").get<std::string>();
				if (priority != PrioritiesLevel::CRITICAL && priority != PrioritiesLevel::HIGHER &&
					priority != PrioritiesLevel::MIDDLE && priority != PrioritiesLevel::LOWER) {
					throw std::runtime_error("Invalid priority. Please enter one of the specified options.");

				}
				// Make sure priority is treated as a string
				bool isOrdered = false;
				if (taskType == TaskType::ORDERED)
					isOrdered = true;

				return std::make_shared<Task>(
					Scheduler::taskIds++,
					taskData.at("priority").get<std::string>(),  // Priority is a string
					taskData.at("runningTime").get<int>(),        // Running time is an integer
					isOrdered
				);
			}
			else {
				throw std::invalid_argument("Invalid Arguments. Please enter one of the specified options.");
			}
		}
		// Check if taskData contains the required keys for a deadline task
		else if (taskType == TaskType::DEAD_LINE) {
			if (taskData.contains("priority") && taskData.contains("runningTime") && taskData.contains("deadline")) {
				auto priority = taskData.at("priority").get<std::string>();
				if (priority != PrioritiesLevel::CRITICAL && priority != PrioritiesLevel::HIGHER &&
					priority != PrioritiesLevel::MIDDLE && priority != PrioritiesLevel::LOWER) {
					throw std::runtime_error("Invalid priority. Please enter one of the specified options.");
				}
				Task basicTask(
					Scheduler::taskIds++,
					taskData.at("priority").get<std::string>(),  // Priority is a string
					taskData.at("runningTime").get<int>()        // Running time is an integer
				);
				int deadLineTime = taskData.at("deadline").get<int>();  // Deadline is an integer

				auto task = std::make_shared<DeadLineTask>(basicTask, deadLineTime);
				return dynamic_pointer_cast<Task>(task);
			}
			else {
				throw std::invalid_argument("Invalid Arguments. Please enter one of the specified options.");
			}
		}
		// Check if taskData contains the required keys for an iterative task
		else if (taskType == TaskType::ITERATIVE) {
			if (taskData.contains("priority") && taskData.contains("runningTime") && taskData.contains("iterationsRemaining") && taskData.contains("executionInterval")) {
				auto priority = taskData.at("priority").get<std::string>();
				if (priority != PrioritiesLevel::CRITICAL && priority != PrioritiesLevel::HIGHER &&
					priority != PrioritiesLevel::MIDDLE && priority != PrioritiesLevel::LOWER) {
					throw std::runtime_error("Invalid priority. Please enter one of the specified options.");
				}
				Task basicTask(
					Scheduler::taskIds++,
					taskData.at("priority").get<std::string>(),  // Priority is a string
					taskData.at("runningTime").get<int>()        // Running time is an integer
				);
				int iterationsRemaining = taskData.at("iterationsRemaining").get<int>();  // Integer
				int executionInterval = taskData.at("executionInterval").get<int>();      // Integer
				auto task = std::make_shared<IterativeTask>(basicTask, iterationsRemaining, executionInterval);
				return dynamic_pointer_cast<Task>(task);
			}
			else {
				throw std::invalid_argument("Invalid Arguments. Please enter one of the specified options.");
			}
		}
		else {
			throw std::runtime_error("Invalid task type!");
		}
	}
	catch (const std::exception& e) {
		// Handle exceptions thrown during JSON parsing or task creation
		std::cerr << "An exception occurred: " << e.what() << std::endl;
		spdlog::error("An exception occurred: {}", e.what());
	}

	return nullptr; // Return nullptr if something goes wrong
}

