#include "Scheduler.h"
#include "TaskFactory.h"
#include "LongTaskHandler.h"

int Scheduler::totalRunningTask = 0;
unsigned int Scheduler::taskIds = 0;
mutex Scheduler::coutMutex;
mutex Scheduler::rtLock;
mutex Scheduler::realTimeQueueMutex;
mutex Scheduler::wrrQueueMutex;
RealTimeScheduler Scheduler::realTimeScheduler;
WeightRoundRobinScheduler Scheduler::wrrQueuesScheduler;
IterativeTaskHandler Scheduler::iterativeTaskHandler;
DeadlineTaskManager Scheduler::deadlineTaskManager;
OrderedTaskHandler Scheduler::orderedTaskHandler;


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
		// Create a thread for the read from json file
		std::thread readtasksFromJSON_Thread([this]() {
			SetThreadDescription(GetCurrentThread(), L"createTasksFromJSON");
			spdlog::info(Logger::LoggerInfo::START_READ_FROM_JSON_THREAD);
			ReadFromJSON::createTasksFromJSON(Scenario::SCENARIO_1_FILE_PATH);
			});

		// Create a thread for the InsertTask function
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

		// Create a thread for Iterative task manager
		std::thread IterativeTaskHandler_Thread([this]() {
			SetThreadDescription(GetCurrentThread(), L"IterativeTaskHandler");
			spdlog::info(Logger::LoggerInfo::START_THREAD, "IterativeTaskHandler");
			this->iterativeTaskHandler.checkTime();
			});

		readtasksFromJSON_Thread.join();
		insertTask_Thread.join();
		RTScheduler_Thread.join();
		WRRScheduler_Thread.join();
		IterativeTaskHandler_Thread.join();
	}
	catch (const std::exception& ex) {
		// Handle any exceptions that might occur during thread creation
		spdlog::error(Logger::LoggerError::ERROR_CREATE_THREAD, ex.what());
	}
}

void Scheduler::insertTaskFromInput()
{
	while (true) {
		cout << "Enter task type. basic/deadLine/iterative/ordered:" << endl;
		string type;
		cin >> type;

		// Validate the input task type
		if (type == TaskType::BASIC || type == TaskType::DEAD_LINE || type == TaskType::ITERATIVE || type == TaskType::ORDERED) {
			shared_ptr<Task> newTask = TaskFactory::createTask(type);
			insertTask(newTask);
		}
		else {
			cout << "Invalid task type." << endl;
		}
	}
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
		spdlog::error(Logger::LoggerError::ERROR_INSERT_TASK);
	}

	else if (newTask->getIsOrdered() && orderedTaskHandler.frontOrderedTask() != newTask) {
		orderedTaskHandler.addOrderedtask(newTask);
	}
	else {
		addTaskToItsQueue(newTask);
		totalRunningTask++;
		LongTaskHandler::addSumOfAllSeconds(newTask->getRunningTime());

		if (shared_ptr< IterativeTask> iterativeTask = dynamic_pointer_cast<IterativeTask>(newTask)) {
			// Check if dynamic_pointer_cast succeeded
			iterativeTaskHandler.pushIterativeTask(iterativeTask);
		}
		else if (shared_ptr< DeadLineTask> deadLineTask = dynamic_pointer_cast<DeadLineTask>(newTask)) {
			// Check if dynamic_pointer_cast succeeded
			deadlineTaskManager.addTask(deadLineTask);
		}
	}
}

void Scheduler::addTaskToItsQueue(shared_ptr<Task> taskToAdd) {
	if (taskToAdd->getPriority() == PrioritiesLevel::CRITICAL) {
		// Lock the mutex for the real-time queue when adding a task
		std::lock_guard<std::mutex> lock(realTimeQueueMutex);
		realTimeScheduler.addTask(taskToAdd); // Add task to real-time scheduler
		spdlog::info(Logger::LoggerInfo::ADD_CRITICAL_TASK, taskToAdd->getId());
	}
	else {
		// Lock the mutex for the WRR queue when adding a task
		std::lock_guard<std::mutex> lock(wrrQueueMutex);
		wrrQueuesScheduler.addTask(taskToAdd); // Add task to WRR scheduler
		spdlog::info(Logger::LoggerInfo::ADD_NON_CRITICAL_TASK, taskToAdd->getId(), taskToAdd->getPriority());
	}
}

/*
 * @brief Pop task From its Q.
 *
 * Thread-safe pop operation for task queues
 *
 */
void Scheduler::popTaskFromItsQueue(shared_ptr<Task> taskToPop) {
	if (taskToPop->getPriority() == PrioritiesLevel::CRITICAL) {
		// Lock the mutex for the real-time queue
		std::lock_guard<std::mutex> lock(realTimeQueueMutex);
		if (!realTimeScheduler.getRealTimeQueue().empty()) {
			realTimeScheduler.getRealTimeQueue().pop();
		}
	}
	else {
		// Lock the mutex for the WRR queue
		std::lock_guard<std::mutex> lock(wrrQueueMutex);
		if (!wrrQueuesScheduler.getWrrQueues()[taskToPop->getPriority()].queue.empty()) {
			wrrQueuesScheduler.getWrrQueues()[taskToPop->getPriority()].queue.pop();
		}
	}
}

/*
 * @brief Executes a given task.
 *
 * This function processes a task by running it and updating its status. If the task is not critical and there are tasks in the real-time scheduler queue, the current task is preempted and a task from the real-time queue is executed. The function also handles exceptions by setting the task status to terminated and printing an error message.
 *
 * @param task Pointer to the task to be executed.
 */
void Scheduler::execute(shared_ptr<Task> task) {

	deadlineTaskManager.deadlineMechanism();
	LongTaskHandler::calculateAverageLength();
	LongTaskHandler::setNumOfSeconds(0);
	spdlog::info(Logger::LoggerInfo::START_EXECUTE, task->getId());
	task->setStatus(TaskStatus::RUNNING);

	// Continue executing the task while it has remaining running time
	while (true) {
		if (task->getRunningTime() == 0) {//the task has finished 
			task->setStatus(TaskStatus::COMPLETED);
			popTaskFromItsQueue(task);
			totalRunningTask--;
			spdlog::info(Logger::LoggerInfo::TASK_COMPLITED, task->getId());
			if (task->getIsOrdered())
				orderedTaskHandler.popOrderedTask();
			break;
		}
		if (LongTaskHandler::haveToSuspendLongTask(task)) {//long task-suspend 
			LongTaskHandler::stopLongTask(task);
			break;
		}
		if (task->getPriority() != PrioritiesLevel::CRITICAL && !realTimeScheduler.getRealTimeQueue().empty()) {//preemptive
			spdlog::info(Logger::LoggerInfo::TASK_PREEMPTITVE, task->getId());
			preemptive(task);
			return;
		}

		try {
			// Simulate task execution by decrementing running time
			task->setRunningTime(task->getRunningTime() - 1);
			LongTaskHandler::increaseNumOfSeconds();
			LongTaskHandler::addSumOfAllSeconds(-1);

			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		catch (const std::exception& e) {
			// Handle any exceptions that occur during execution
			spdlog::error(Logger::LoggerError::TASK_TERMINATED, task->getId(), e.what());
			task->setStatus(TaskStatus::TERMINATED);
			popTaskFromItsQueue(task);
			totalRunningTask--;
			LongTaskHandler::addSumOfAllSeconds(task->getRunningTime());
			break; // Exit the loop if an exception is caught
		}
	}
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
	spdlog::info(Logger::LoggerInfo::TASK_SUSPENDED, task->getId());

}

/**
 * @brief Displays the status of a task.
 *
 * This function prints the current status of the specified task to the standard output.
 *
 * @param task Pointer to the task whose status is to be displayed.
 */
void Scheduler::displayMessage(const Task* task) {
	printAtomically("task " + to_string(task->getId()) + " with priority: " + task->getPriority() + " and running time " + std::to_string(task->getRunningTime()) + " is " + task->getStatus() + "\n");
}

void Scheduler::printAtomically(const string& message) {
	std::lock_guard<std::mutex> lock(coutMutex);
	std::cout << message;
}

RealTimeScheduler& Scheduler::getRealTimeScheduler() {
	return realTimeScheduler;
}

WeightRoundRobinScheduler& Scheduler::getWrrQueuesScheduler() {
	return wrrQueuesScheduler;
}

IterativeTaskHandler& Scheduler::getIterativeTaskHandler() {
	return iterativeTaskHandler;
}

DeadlineTaskManager& Scheduler::getDeadlineTaskManager() {
	return deadlineTaskManager;
}

OrderedTaskHandler& Scheduler::getOrderedTaskHandler() {
	return orderedTaskHandler;
}

