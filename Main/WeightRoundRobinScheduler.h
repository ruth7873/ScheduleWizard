#pragma once
#include <queue>
#include "Task.h"
#include "Consts.h"
#include <unordered_map>

typedef struct Queue {
	queue<Task*> WRRQueue;
	int weight;
};

#pragma once
class WeightRoundRobinScheduler
{
private:
	int countTasks;
	unordered_map<string, Queue> WRRQueues;

public:
	WeightRoundRobinScheduler();
	void addTask(Task*);

};