
#pragma once
#include <queue>
#include "Task.h"
#include "Consts.h"
#include <unordered_map>

typedef struct Queue {
	static unordered_map<string, queue<Task*>> WRRQueues;
	int weight;
};

#pragma once
class WeightRoundRobinScheduler
{
private:
	int countTasks;
	unordered_map<string, Queue> WRRQueues;
	unordered_map<string, int> WRRQueuesWeights;
};
