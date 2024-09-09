#include "Scheduler.h"
//#include "DeadlineTask.h"
//#include "DeadlineTaskManager.h"
#include "TaskFactory.h"
#include "LongTaskHandler.h"

int Scheduler::totalRunningTask = 0;
unsigned int Scheduler::taskIds = 0;
mutex Scheduler::coutMutex;
mutex Scheduler::rtLock;
RealTimeScheduler Scheduler::realTimeScheduler;
WeightRoundRobinScheduler Scheduler::wrrQueuesScheduler;
DeadlineTaskManager Scheduler::deadlineTaskManager;
mutex Scheduler::realTimeQueueMutex;
mutex Scheduler::wrrQueueMutex;

// Thread-safe pop operation for task queues
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
	spdlog::info("Executing task with ID: {}", task->getId());
	task->setStatus(TaskStatus::RUNNING);

	// Continue executing the task while it has remaining running time
	while (true) {
		if (task->getRunningTime() == 0) {//the task has finished 

			task->setStatus(TaskStatus::COMPLETED);
			popTaskFromItsQueue(task);
			totalRunningTask--;
			spdlog::info("Task with ID: {} completed.", task->getId());
			cout << "the size of the Q "<< getWrrQueuesScheduler().getWrrQueues().size()<< endl;
			break;
		}
		if (LongTaskHandler::haveToSuspendLongTask(task)) {//long task-suspend 
			LongTaskHandler::stopLongTask(task);
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
			LongTaskHandler::increaseNumOfSeconds();
			LongTaskHandler::addSumOfAllSeconds(-1);
			cout << "the Running time: " << task->getRunningTime() << " of task id: " << task->getId() << endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		catch (const std::exception& e) {
			// Handle any exceptions that occur during execution
			spdlog::error("Exception occurred while executing task with ID: {}: {}", task->getId(), e.what());
			task->setStatus(TaskStatus::TERMINATED);
			popTaskFromItsQueue(task);
			totalRunningTask--;
			break; // Exit the loop if an exception is caught
		}
	}

    //task->setStatus(TaskStatus::COMPLETED);


	/* if (task != nullptr) {
		 delete task;
	 }*/

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
			ReadFromJSON::createTasksFromJSON(Scenario::SCENARIO_1_FILE_PATH);
			});	// Create a thread for the InsertTask function

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
		
		readtasksFromJSON_Thread.join();
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
		cout << "Enter task type. basic/deadLine/iterative:" << endl;
		string type;
		cin >> type;

		if (type == "aa"){
			shared_ptr<Task> newTask = TaskFactory::createTask(TaskType::DEAD_LINE);
			insertTask(newTask);
		}
		// Validate the input task type
		if (type == TaskType::BASIC || type == TaskType::DEAD_LINE || type == TaskType::ITERATIVE ) {
			shared_ptr<Task> newTask = TaskFactory::createTask(type);
			insertTask(newTask);
		}
		else {
			cout << "Invalid task type. Please enter 'basic', 'deadline', or 'iterative'." << endl;
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
		std::cout << "Error: Invalid task input. Please try again." << std::endl;
		spdlog::error("Error: Invalid task input. Skipping task insertion.");
	}
	addTaskToItsQueue(newTask);
	totalRunningTask++;
	LongTaskHandler::addSumOfAllSeconds(newTask->getRunningTime());
}


void Scheduler::printAtomically(const string& message) {
	std::lock_guard<std::mutex> lock(coutMutex);
	std::cout << message;
}
