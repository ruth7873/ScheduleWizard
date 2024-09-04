#pragma once
#include <iostream>
#include <thread>
#include <chrono>    
#include <string>
#include <memory>
#include <mutex>
#include "Task.h"
#include "RealTimeScheduler.h"
#include "WeightRoundRobinScheduler.h"
#include "Logger.h"
#include "Consts.h"
#include "ReadFromJSON.h"

class RealTimeScheduler;
class Task;
class WeightRoundRobinScheduler;

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
	static WeightRoundRobinScheduler wrrQueues;

public:
	static const unsigned int MAX_TASKS = std::numeric_limits<unsigned int>::max();

	static mutex rtLock;
	static int totalRunningTask;
	static unsigned int taskIds;

	static shared_ptr<Task> input();
	void init();
	void insertTaskFromInput();

	static void insertTask(shared_ptr<Task>);
	static void execute(shared_ptr<Task> task);
	static void displayMessage(Task* task);
	static void preemptive(shared_ptr<Task> task);
};
