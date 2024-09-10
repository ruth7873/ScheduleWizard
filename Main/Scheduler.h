#pragma once
#include <thread>
#include <chrono>    
#include <string>
#include <memory>
#include <mutex>
#include <iostream>
#include "Task.h"
#include "Logger.h"
#include "Consts.h"
#include "ReadFromJSON.h"
#include "RealTimeScheduler.h"
#include "DeadlineTaskManager.h"
#include "IterativeTaskHandler.h"
#include "WeightRoundRobinScheduler.h"

class RealTimeScheduler;
class Task;
class WeightRoundRobinScheduler;
class DeadlineTaskManager;

/**
 * @class Scheduler
 * @brief Manages task scheduling and execution.
 *
 * The Scheduler class provides functionalities for task execution, task insertion, and scheduling. It handles tasks based on their priority and uses multiple schedulers to manage execution.
 */
class Scheduler
{
private:
	static RealTimeScheduler realTimeScheduler;
	static IterativeTaskHandler iterativeTaskHandler;
	static WeightRoundRobinScheduler wrrQueuesScheduler;
	static DeadlineTaskManager deadlineTaskManager;
	static mutex coutMutex;
	static mutex realTimeQueueMutex;
	static mutex wrrQueueMutex;

public:
	static const unsigned int MAX_TASKS = std::numeric_limits<unsigned int>::max();
	static mutex rtLock;
	static int totalRunningTask;
	static unsigned int taskIds;

	void init();
	void insertTaskFromInput();

	static void printAtomically(const string& message);
	static void insertTask(shared_ptr<Task>);
	static void execute(shared_ptr<Task> task);
	static void displayMessage(const Task* task);
	static void preemptive(shared_ptr<Task> task);

	static void popTaskFromItsQueue(shared_ptr<Task> taskToPop);
	static void addTaskToItsQueue(shared_ptr<Task> taskToAdd);

	static RealTimeScheduler& getRealTimeScheduler();
	static WeightRoundRobinScheduler& getWrrQueuesScheduler();
	static IterativeTaskHandler& getIterativeTaskHandler();
	static DeadlineTaskManager& getDeadlineTaskManager();
};
