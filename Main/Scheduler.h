#pragma once
#include <iostream>
#include <thread>
#include <chrono>    
#include <string>
#include "Task.h"
#include "RealTimeScheduler.h"
#include "WeightRoundRobinScheduler.h"
  

class RealTimeScheduler;

class Scheduler
{
private:

	static RealTimeScheduler realTimeScheduler;
	static WeightRoundRobinScheduler wrrQueues;
	Task* Input();

public:

	void StartScheduling();
	void InsertTask();
	static int taskAmount;
 	static void execute(Task* task);
	static void displayMessage(const Task* task, string);
	static void preemptive(Task* task);

};

