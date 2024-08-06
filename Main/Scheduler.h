
#pragma once

#include "Task.h"
#include "Consts.h"
#include "RealTimeScheduler.h"
#include "WeightRoundRobinScheduler.h"
#include <iostream>

class Scheduler
{
private:
	RealTimeScheduler realTime;

	WeightRoundRobinScheduler WrrQueues;

	Task* currentTask;

public:

};

