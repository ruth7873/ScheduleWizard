
#pragma once

#include "Task.h"
#include "Consts.h"
#include "RealTimeScheduler.h"
#include "WeightRoundRobinScheduler.h"
#include <iostream>
#include <thread>
#include <chrono>    
#include <string>

class RealTimeScheduler;

class Scheduler
{
private:
	static RealTimeScheduler realTimeScheduler;
	static WeightRoundRobinScheduler WrrQueues;
public:
	static void execute(Task* task);
	static void displayMessage(const Task* task);
	static void preemptive(Task* task);
	static int taskAmount;
};


