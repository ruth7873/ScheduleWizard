#pragma once
#include <queue>
#include <unordered_map>
#include <string>
#include <memory>
#include "Task.h"
#include "Consts.h"
#include "queue"
#include "Scheduler.h"
#include <thread>
#include <chrono>
#include "Timer.h"

class Task;
typedef struct Q {
	queue<shared_ptr<Task>> queue;
	int weight;
} Queue;

class WeightRoundRobinScheduler
{
private:
	std::unordered_map<std::string, Queue> WRRQueues;

public:
	//c'tors/d'tors
	WeightRoundRobinScheduler();
	WeightRoundRobinScheduler(WeightRoundRobinScheduler&& wrr) = delete;
	WeightRoundRobinScheduler(const WeightRoundRobinScheduler& wrr) = delete;
	~WeightRoundRobinScheduler();

	void addTask(shared_ptr<Task> task);
	void weightRoundRobinFunction();
	std::unordered_map<std::string, Queue>& getWrrQueues();
};