
#pragma once
#include <queue>
#include "Task.h"
#include "Consts.h"
#include <unordered_map>

typedef struct Queue {
	queue<Task*> WRRQueues;
	int weight;
};

#pragma once
class WeightRoundRobinScheduler
{
private:
	int countTasks;
	unordered_map<string, Queue> WRRQueues;
	unordered_map<string, int> WRRQueuesWeights;
public:
	void addTask(Task* task) {
		WRRQueues[task->getPriority()].WRRQueues.push(task);
	}
	unordered_map<string, Queue> getWrrQueues() {
		return WRRQueues;
	}
};
