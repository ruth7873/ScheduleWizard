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
	static int sumOfAllSeconds;
	static int numOfSeconds;
	static double AverageLength;
	static RealTimeScheduler realTimeScheduler;
	static WeightRoundRobinScheduler wrrQueuesScheduler;
	const unsigned int MAX_TASKS = std::numeric_limits<unsigned int>::max();
	static void popTaskFromItsQueue(shared_ptr<Task> taskToPop);
	static void addTaskToItsQueue(shared_ptr<Task> taskToAdd);
	static void stopLongTask(shared_ptr<Task>);
	shared_ptr<Task> input();
	static void calculateAverageLength();
	static std::mutex coutMutex;

public:
	static mutex rtLock;
	static int totalRunningTask;
	static unsigned int taskIds;

	void init();
	void insertTaskFromInput();

	static void insertTask(shared_ptr<Task>);
	static void execute(shared_ptr<Task> task);
	static void displayMessage(const Task* task);
	static void preemptive(shared_ptr<Task> task);
};
