#include "RealTimeScheduler.h"
#include "Scheduler.h"
#include "Utilities.h"

RealTimeScheduler::~RealTimeScheduler(){
	while(!realTimeQueue.empty()){
		delete realTimeQueue.front();
		realTimeQueue.pop();
	}

}
/**
	 * @brief Function that manages the execution of real-time tasks.
	 *
	 * This function runs in an infinite loop. It checks if the real-time queue is not empty, and if it is not, it retrieves the first task from the queue and passes it to the `Scheduler::execute` function for processing.
	 */
void RealTimeScheduler::realTimeSchedulerFunction() {

	while (true) {
		//while (realTimeQueue.empty());
		auto startTime = std::chrono::steady_clock::now();
		while (realTimeQueue.empty()) {
			if (checkLoopTimeout(startTime, 10)) {  // בדיקה של תקיעה בלולאה עם Timeout של 5 דקות
				break;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		if (realTimeQueue.empty()) continue;
		Task* task = RealTimeScheduler::realTimeQueue.front();
		RealTimeScheduler::realTimeQueue.pop();
		spdlog::info("Popped task with ID: {} from real-time queue.", task->getId());
		spdlog::info("real-time queue size is {}.", realTimeQueue.size());

		Scheduler::rtLock.lock();

		Scheduler::execute(task);
		if (realTimeQueue.empty())
			Scheduler::rtLock.unlock();
	}
}
