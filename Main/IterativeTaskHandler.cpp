#include "IterativeTaskHandler.h"
#include <chrono>
#include "Scheduler.h"
#include "IterativeTask.h"

//class IterativeTask;

void IterativeTaskHandler::pushIterativeTask(shared_ptr<IterativeTask> iterativeTask)
{
	int iterationsRemaining = iterativeTask.get()->getIterationsRemaining();
	if (iterationsRemaining <= 1) {
		return;
	}

	//decrease the one remaie
	iterativeTask.get()->dereaseIterationsRemaining();

	//add to waitUnitTime the time that need to wait between tasks
	auto systemTime = std::chrono::system_clock::now();
	std::time_t currentTime_t = std::chrono::system_clock::to_time_t(systemTime);
	int waitTime = iterativeTask.get()->getExecutionInterval() + currentTime_t;
	iterativeTask.get()->setWaitTime(waitTime);
	minHeap.emplace(iterativeTask);
}

shared_ptr<IterativeTask> IterativeTaskHandler::popIterativeTask()
{
	shared_ptr<IterativeTask> topTask = minHeap.top();
	minHeap.pop();

	topTask.get()->setRunningTime(topTask.get()->getRunTime());

	return topTask;
}

void IterativeTaskHandler::checkTime()
{
	while (true) {
		auto systemTime = std::chrono::system_clock::now();
		std::time_t currentTime_t = std::chrono::system_clock::to_time_t(systemTime);
		
		if (!this->isEmpty()) {
			std::time_t waitTime = minHeap.top()->getWaitTime();

			if (currentTime_t >= waitTime) {
				Scheduler::insertTask(dynamic_pointer_cast<Task>(popIterativeTask()));
			}
		}
	}
}



