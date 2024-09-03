#include "Scheduler.h"

int Scheduler::sumOfAllSeconds = 0;
int Scheduler::totalRunningTask = 0;
unsigned int Scheduler::taskIds = 0;
int Scheduler::numOfSeconds = 0;
double Scheduler::AverageLength = 0;
mutex Scheduler::coutMutex;
mutex Scheduler::rtLock;
RealTimeScheduler Scheduler::realTimeScheduler;
WeightRoundRobinScheduler Scheduler::wrrQueuesScheduler;

void Scheduler::popTaskFromItsQueue(shared_ptr<Task> taskToPop) {
	if (taskToPop->getPriority() == PrioritiesLevel::CRITICAL && !realTimeScheduler.getRealTimeQueue().empty()) {
		realTimeScheduler.getRealTimeQueue().pop();
	}
	else if (!wrrQueuesScheduler.getWrrQueues()[taskToPop->getPriority()].queue.empty()) {
		wrrQueuesScheduler.getWrrQueues()[taskToPop->getPriority()].queue.pop();
	}
	totalRunningTask--;
}

void Scheduler::addTaskToItsQueue(shared_ptr<Task> taskToAdd) {
	if (taskToAdd->getPriority() == PrioritiesLevel::CRITICAL) {
		realTimeScheduler.addTask(taskToAdd); // Add task to real-time scheduler for real-time tasks
		spdlog::info(Logger::LoggerInfo::ADD_CRITICAL_TASK, taskToAdd->getId());
	}
	else {
		wrrQueuesScheduler.addTask(taskToAdd); // Add task to Weighted Round Robin scheduler for non-real-time tasks
		spdlog::info(Logger::LoggerInfo::ADD_NON_CRITICAL_TASK, taskToAdd->getId(), taskToAdd->getPriority());
	}
}

void Scheduler::stopLongTask(shared_ptr<Task> longTask) {
	longTask->setStatus(TaskStatus::SUSPENDED);
	spdlog::info("the long task stop, and will continue later");
	popTaskFromItsQueue(longTask);
	addTaskToItsQueue(longTask);
}

void Scheduler::calculateAverageLength() {
	if (totalRunningTask)
		AverageLength = sumOfAllSeconds / totalRunningTask;
	std::ostringstream oss;
	oss << "the average is: " << AverageLength << endl;
	std::lock_guard<std::mutex> lock(coutMutex);
	std::cout << oss.str();

}
/**
 * @brief Executes a given task.
 *
 * This function processes a task by running it and updating its status. If the task is not critical and there are tasks in the real-time scheduler queue, the current task is preempted and a task from the real-time queue is executed. The function also handles exceptions by setting the task status to terminated and printing an error message.
 *
 * @param task Pointer to the task to be executed.
 */
void Scheduler::execute(shared_ptr<Task> task) {
	calculateAverageLength();
	numOfSeconds = 0;
	spdlog::info("Executing task with ID: {}", task->getId());
	task->setStatus(TaskStatus::RUNNING);
	// Continue executing the task while it has remaining running time
	while (true) {
		if (task->getRunningTime() == 0) {//the task has finished 
			// Set the task status to COMPLETED when execution is finished
			task->setStatus(TaskStatus::COMPLETED);
			popTaskFromItsQueue(task);
			spdlog::info("Task with ID: {} completed.", task->getId());
			break;
		}
		if (numOfSeconds > AverageLength) {//long task-suspend
			stopLongTask(task);
			break;
		}
		if (task->getPriority() != PrioritiesLevel::CRITICAL && !realTimeScheduler.getRealTimeQueue().empty()) {//preemptive
			spdlog::info("Preempting task with ID: {} for real-time task.", task->getId());
			preemptive(task);
			return;
		}
		try {
			// Simulate task execution by decrementing running time
			task->setRunningTime(task->getRunningTime() - 1);
			numOfSeconds++;
			sumOfAllSeconds--;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		catch (const std::exception& e) {
			// Handle any exceptions that occur during execution
			spdlog::error("Exception occurred while executing task with ID: {}: {}", task->getId(), e.what());
			task->setStatus(TaskStatus::TERMINATED);
			popTaskFromItsQueue(task);
			break; // Exit the loop if an exception is caught
		}
	}
}

/**
 * @brief Displays the status of a task.
 *
 * This function prints the current status of the specified task to the standard output.
 *
 * @param task Pointer to the task whose status is to be displayed.
 */
void Scheduler::displayMessage(const Task* task) {
	std::ostringstream oss;
	oss << "task " << task->getId()
		<< " with priority: " << task->getPriority()
		<< " and running time: " << task->getRunningTime()
		<< " is " << task->getStatus() << std::endl;

	std::lock_guard<std::mutex> lock(coutMutex);
	std::cout << oss.str();
	//std::cout << "task " << task->getId() << " with priority: " << task->getPriority() <<"and running time "<<task->getRunningTime() << " is " << task->getStatus() << std::endl;
}

/**
 * @brief Handles task preemption.
 *
 * This function sets the status of the given task to suspended and adds it to the weighted round-robin queue. This is used to preempt the current task when a higher priority task needs to be processed.
 *
 * @param task Pointer to the task to be preempted.
 */
void Scheduler::preemptive(shared_ptr<Task> task) {
	task->setStatus(TaskStatus::SUSPENDED);
	spdlog::info("Task with ID: {} suspended and added back to WRR queue.", task->getId());
}

/**
 * @brief Initiates the scheduling process by creating and managing threads for various scheduler functions.
 *
 * This function creates separate threads for inserting tasks, real-time scheduling, and Weighted Round Robin scheduling.
 * It utilizes threading to allow concurrent execution of these tasks in the Scheduler class context.
 * Exceptions that may occur during thread creation are caught and handled within the function.
 */
void Scheduler::init() {
	Logger::initialize_logger();

	spdlog::info(Logger::LoggerInfo::START_SCHEDULER);
	try {
		std::thread readtasksFromJSON_Thread([this]() {
			SetThreadDescription(GetCurrentThread(), L"createTasksFromJSONWithDelay");
			spdlog::info("read tasks From JSON thread started.");
			ReadFromJSON::createTasksFromJSONWithDelay(Scenario::SCENARIO_1_FILE_PATH, 3, 15);
			});		// Create a thread for the InsertTask function
		std::thread insertTask_Thread([this]() {
			SetThreadDescription(GetCurrentThread(), L"InsertTask");
			spdlog::info(Logger::LoggerInfo::START_THREAD, "InsertTask");
			this->insertTaskFromInput();
			});

		// Create a thread for real-Time Scheduler
		std::thread RTScheduler_Thread([this]() {
			SetThreadDescription(GetCurrentThread(), L"RealTimeScheduler");
			spdlog::info(Logger::LoggerInfo::START_THREAD, "RealTimeScheduler");
			realTimeScheduler.realTimeSchedulerFunction();
			});

		// Create a thread for WRR Scheduler
		std::thread WRRScheduler_Thread([this]() {
			SetThreadDescription(GetCurrentThread(), L"WeightRoundRobinScheduler");
			spdlog::info(Logger::LoggerInfo::START_THREAD, "WeightRoundRobinScheduler");
			wrrQueuesScheduler.weightRoundRobinFunction();
			});

		insertTask_Thread.join();
		RTScheduler_Thread.join();
		WRRScheduler_Thread.join();
	}
	catch (const std::exception& ex) {
		// Handle any exceptions that might occur during thread creation
		spdlog::error(Logger::LoggerError::ERROR_CREATE_THREAD, ex.what());
	}
}

void Scheduler::insertTaskFromInput()
{
	while (true) {
		insertTask(input());
	}
}


/**
 * @brief This function allows the user to input details for a new task, including priority and running time.
 *
 * @return A pointer to the newly created Task object based on the user input.
 */
shared_ptr<Task> Scheduler::input()
{
	std::string priority;
	int runningTime;
	std::string input;

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
	while (true) {
		std::cout << "Enter the task running time in seconds: \n";
		std::cin >> input;

		// Check if the input contains only digits
		if (input.find_first_not_of("0123456789-") != std::string::npos) {
			std::cerr << "Invalid input. Running time should contain only numeric digits." << std::endl;
			continue; // Restart the loop for a valid input
		}

		// Convert the input to an integer
		runningTime = std::stoi(input);

		if (runningTime < 0) {
			std::cerr << "Invalid input: Running time cannot be negative." << std::endl;
			continue; // Restart the loop for a valid input
		}

		break; // If runningTime is valid, exit the loop
	}
	spdlog::info(Logger::LoggerInfo::CREATE_NEW_TASK, priority, runningTime);
	// Assuming other fields like status and entryTime are set elsewhere

	return shared_ptr<Task>(new Task((taskIds++) % MAX_TASKS, priority, runningTime));
}

/**
 * @brief Continuously prompts the user to input task details and inserts the tasks into the appropriate scheduler.
 *
 * This function loops indefinitely, prompting the user to input task details using the Input function.
 * If the input task is valid, it is added to the corresponding scheduler based on its priority level.
 * Tasks with Critical priority are added to the real-time scheduler, while others are added to the Weighted Round Robin scheduler.
 */

void Scheduler::insertTask(shared_ptr<Task> newTask)
{
	if (newTask == nullptr) {
		std::cerr << "Error: Invalid task input. Please try again." << std::endl;
		spdlog::error("Error: Invalid task input. Skipping task insertion.");
	}
	addTaskToItsQueue(newTask);
	totalRunningTask++;
	sumOfAllSeconds += newTask->getRunningTime();
}
