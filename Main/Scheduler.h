
#pragma once

#include "Task.h"
#include "RealTimeScheduler.h"
#include "WeightRoundRobinScheduler.h"
#include <iostream>

class Scheduler
{
private:
	static RealTimeScheduler realTime;

	static WeightRoundRobinScheduler wrrQueues;

	static Task* currentTask;

	Task* Input();
public:

	void StartScheduling();

	void InsertTask();

	static int taskAmount;

};

