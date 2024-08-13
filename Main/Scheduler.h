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
    static int taskCounter;
	static RealTimeScheduler realTimeScheduler;
	static WeightRoundRobinScheduler wrrQueues;
	Task* Input();

public:

    static mutex rtLock;

    void StartScheduling();
    void InsertTaskFromInput();
    static void InsertTask(Task*);
    static int taskAmount;
    static int totalTasks;
    static void execute(Task* task);
    static void displayMessage(const Task* task);
    static void preemptive(Task* task);
};
