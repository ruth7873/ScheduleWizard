#include "RealTimeScheduler.h"

RealTimeScheduler::~RealTimeScheduler() {
	while (!realTimeQueue.empty()) {
		realTimeQueue.pop();
	}
}

/**
* @brief Function that manages the execution of real-time tasks.
*
* This function runs in an infinite loop. It checks if the real-time queue is not empty, and if it is not, it retrieves the first task from the queue and passes it to the `Scheduler::execute` function for processing.
*/
void RealTimeScheduler::realTimeSchedulerFunction() {
	while (true)
	{
		while (realTimeQueue.empty());

		std::unique_lock<std::mutex> lock(Scheduler::rtLock);

		shared_ptr<Task> task = realTimeQueue.front();

		if (task != nullptr) {
			if (task->getStatus() != TaskStatus::RUNNING && task->getStatus() != TaskStatus::COMPLETED) {
				Scheduler::execute(task);
			}
			else {
				if (!realTimeQueue.empty()&& task->getStatus() == TaskStatus::COMPLETED) {
					realTimeQueue.pop();
				}
			}
		}
	}
}

queue<shared_ptr<Task>>& RealTimeScheduler::getRealTimeQueue() {
	return realTimeQueue;
}

void RealTimeScheduler::addTask(shared_ptr<Task> task) {
	realTimeQueue.push(task);
}