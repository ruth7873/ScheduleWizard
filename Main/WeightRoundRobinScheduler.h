#pragma once
#include <queue>
#include "Task.h"
#include "Consts.h"
#include <unordered_map>

typedef struct Q {
	queue<Task*> WRRQueue;
	int weight;
}Queue;


class WeightRoundRobinScheduler
{
private:
	unordered_map<string, Queue> WRRQueues;

public:
		unordered_map<string, Queue> getWrrQueues() {
		return WRRQueues;
	}
	WeightRoundRobinScheduler();
	void addTask(Task*);

};

