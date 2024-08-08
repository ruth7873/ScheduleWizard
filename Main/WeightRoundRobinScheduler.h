#pragma once
#include <queue>
#include "Task.h"
#include "Consts.h"
#include <unordered_map>
#include <string>

<<<<<<< HEAD
typedef struct Queue {
	queue<Task*> queue;
	int weight;
};
=======
typedef struct Q {
    std::queue<Task*> WRRQueue;
    int weight;
} Queue;
>>>>>>> 3e0ceb65378f524166931a6836efd8d2f791065a

class WeightRoundRobinScheduler
{
private:
<<<<<<< HEAD
	std::unordered_map<std::string, Queue> WRRQueues;

public:
	WeightRoundRobinScheduler();
	~WeightRoundRobinScheduler();
	WeightRoundRobinScheduler(WeightRoundRobinScheduler&& wrr) = delete;
	WeightRoundRobinScheduler(const WeightRoundRobinScheduler& wrr) = delete;

	void addTask(Task* task);
	std::unordered_map< std::string, Queue> getWrrQueues();
	void WeightRoundRobin();

=======
    std::unordered_map<std::string, Queue> WRRQueues;

public:
    std::unordered_map<std::string, Queue> getWrrQueues() {
        return WRRQueues;
    }

    WeightRoundRobinScheduler();
    void addTask(Task* task);
>>>>>>> 3e0ceb65378f524166931a6836efd8d2f791065a
};

