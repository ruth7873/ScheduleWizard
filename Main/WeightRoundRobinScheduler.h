#pragma once
#include <queue>
#include "Task.h"
#include "Consts.h"
#include <unordered_map>
#include <string>
#include <memory>



typedef struct Q {
	queue<shared_ptr<Task>> queue;
    int weight;
} Queue;

class WeightRoundRobinScheduler
{
private:
	std::unordered_map<std::string, Queue> WRRQueues;

public:
	WeightRoundRobinScheduler();
	~WeightRoundRobinScheduler();
	WeightRoundRobinScheduler(WeightRoundRobinScheduler&& wrr) = delete;
	WeightRoundRobinScheduler(const WeightRoundRobinScheduler& wrr) = delete;

	void addTask(shared_ptr<Task> task);
	std::unordered_map< std::string, Queue>& getWrrQueues();
	void weightRoundRobinFunction();
};

