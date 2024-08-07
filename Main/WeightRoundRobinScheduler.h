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
private:
	static std::unordered_map<std::string, Queue> WRRQueues;
	std::unordered_map<std::string, int> WRRQueuesWeights;


public:
	WeightRoundRobinScheduler();
	void addTask(Task* task);
	std::unordered_map< std::string, Queue> getWrrQueues();
	void WeightRoundRobin();

};

