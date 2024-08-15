#pragma once
#include <iostream>
#include <thread>
#include <chrono>    
#include <string>
#include "Task.h"
#include "RealTimeScheduler.h"
#include "WeightRoundRobinScheduler.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"
#include <mutex>
#include "Logger.h"

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
	Task* input();
    const unsigned int MAX_TASKS = std::numeric_limits<unsigned int>::max();
public:
    static mutex rtLock;
    static int totalRunningTask;
    static unsigned int taskIds;
    
    void init();
    void insertTaskFromInput();
    static void insertTask(Task*);
    static void execute(Task* task);
    static void displayMessage(const Task* task);
    static void preemptive(Task* task);
};
