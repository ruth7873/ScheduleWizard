#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "Task.h"
#include "Consts.h"
#include "queue"
#include "Scheduler.h"
#include <thread>
#include <chrono>
#include "queue"
#include <thread>
#include <chrono>
#include "IterativeTask.h"

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
	std::unordered_map< std::string, Queue>& getWrrQueues();
	void weightRoundRobinFunction();
};