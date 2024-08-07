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
	std::unordered_map<std::string, Queue> WRRQueues;

public:
	WeightRoundRobinScheduler();
	void addTask(Task* task);
	std::unordered_map< std::string, Queue> getWrrQueues();
	void WeightRoundRobin();

};

