#include "IterativeTaskHandler.h"
#include <chrono>
#include "Scheduler.h"
#include "IterativeTask.h"

/**
 * @brief Pushes an iterative task onto the heap
 *
 * @param iterativeTask The iterative task to be pushed onto the heap
 */
void IterativeTaskHandler::pushIterativeTask(shared_ptr<IterativeTask> iterativeTask)
{
	// Check if there are iterations remaining for the task
	int iterationsRemaining = iterativeTask.get()->getIterationsRemaining();
	if (iterationsRemaining <= 1) {
		return;
	}

	// Decrease the remaining iterations count
	iterativeTask.get()->decreaseIterationsRemaining();

	// Calculate the time to wait before executing the task again
	auto systemTime = std::chrono::system_clock::now();
	std::time_t currentTime_t = std::chrono::system_clock::to_time_t(systemTime);
	int waitTime = iterativeTask.get()->getExecutionInterval() + currentTime_t;
	iterativeTask.get()->setWaitTime(waitTime);

	// Add the task to the min heap
	minHeap.emplace(iterativeTask);
}

/**
 * @brief Pops the top task from the heap
 *
 * @return The top task popped from the heap
 */
shared_ptr<IterativeTask> IterativeTaskHandler::popIterativeTask()
{
	if (isEmpty())
		throw "can't pop from empty heap";
	shared_ptr<IterativeTask> topTask = minHeap.top();
	minHeap.pop();

	// Set the running time of the task
	topTask.get()->setRunningTime(topTask.get()->getRunTime());

	return topTask;
}

/**
 * @brief Checks the current time and executes tasks if their wait time has elapsed
 */
void IterativeTaskHandler::checkTime()
{
	while (true) {
		auto systemTime = std::chrono::system_clock::now();
		std::time_t currentTime_t = std::chrono::system_clock::to_time_t(systemTime);

		// Check if the heap is not empty
		if (!this->isEmpty()) {
			std::time_t waitTime = minHeap.top()->getWaitTime();

			// If the current time is greater than or equal to the task's wait time, execute the task
			if (currentTime_t >= waitTime) {
				Scheduler::insertTask(dynamic_pointer_cast<Task>(popIterativeTask()));
			}
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
