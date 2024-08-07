
#pragma once

#include "Task.h"
#include "Consts.h"
#include "RealTimeScheduler.h"
#include "WeightRoundRobinScheduler.h"
#include <iostream>
#include <thread>
#include <chrono>    

class RealTimeScheduler;

class Scheduler
{
private:
	static RealTimeScheduler realTimeScheduler;
	static WeightRoundRobinScheduler WrrQueues;
	static Task* currentTask;
	Task* Input();
public:
	static void execute(Task* task);
	static void displayMessage(Task* task, string message = "");
	void InsertTask();
	static int taskAmount;
	void rt();
};


