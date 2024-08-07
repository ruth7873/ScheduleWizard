
#pragma once

#include "Task.h"
#include "RealTimeScheduler.h"
#include "WeightRoundRobinScheduler.h"
#include <iostream>
#include <thread>
#include <chrono>    
#include <string>

class Task;
class RealTimeScheduler;
class WeightRoundRobinScheduler;

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
	static void displayMessage(const Task* task);
	static void preemptive(Task* task);

};


