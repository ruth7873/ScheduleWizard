#pragma once
#include <queue>
#include "Task.h"
#include "Scheduler.h"


class Scheduler; // Forward declaration

/**
 * @class RealTimeScheduler
 * @brief Manages real-time tasks.
 *
 * This class is responsible for managing tasks that need to be executed in real-time. It maintains a queue of tasks and processes them according to their order.
 */
class RealTimeScheduler
{
private:
	queue<shared_ptr<Task>> realTimeQueue;

public:
	RealTimeScheduler() {}
	RealTimeScheduler(const RealTimeScheduler& rt) = delete;
	RealTimeScheduler(RealTimeScheduler&& rt) = delete;
	~RealTimeScheduler();

	queue<shared_ptr<Task>>& getRealTimeQueue() {
		return realTimeQueue;
	}
    void addTask(shared_ptr<Task> task) {
        realTimeQueue.push(task);
    }
    void realTimeSchedulerFunction();
};
