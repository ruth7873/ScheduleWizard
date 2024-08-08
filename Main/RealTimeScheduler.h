#pragma once
#include <queue>
#include "Task.h"
#include "Scheduler.h"

/**
 * @class RealTimeScheduler
 * @brief Manages real-time tasks.
 *
 * This class is responsible for managing tasks that need to be executed in real-time. It maintains a queue of tasks and processes them according to their order.
 */
class RealTimeScheduler
{
private:
	queue<Task*> realTimeQueue;

public:
	queue<Task*> getRealTimeQueue() const {
		return realTimeQueue;
	}
    void addTask(Task* task) {
        realTimeQueue.push(task);
    }
    void realTimeSchedulerFunction();
};
