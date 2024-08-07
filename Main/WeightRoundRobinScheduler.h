
#pragma once
#include <queue>
#include "Task.h"
#include "Consts.h"
#include <unordered_map>
#include <string>

typedef struct Queue {
	queue<Task*> queue;
	int weight;
};

#pragma once
class WeightRoundRobinScheduler
{
public:
	WeightRoundRobinScheduler();

private:
	static std::unordered_map<std::string, Queue> WRRQueues;
	unordered_map<string, int> WRRQueuesWeights;
	void WeightRoundRobin();

};
