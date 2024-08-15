#include "RealTimeScheduler.h"
#include "Scheduler.h"

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
		while (realTimeQueue.empty());
		Task* task = nullptr;
		Scheduler::rtLock.try_lock();
			
		task = RealTimeScheduler::realTimeQueue.front();

    if (task != nullptr) {
			Scheduler::execute(task);
		}

		if (realTimeQueue.empty())
			Scheduler::rtLock.unlock();
	}
}
