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
	cout << "start real-time Tread\n";
	while (true) {
		while (realTimeQueue.empty());
		Task* task = RealTimeScheduler::realTimeQueue.front();
		RealTimeScheduler::realTimeQueue.pop();
		Scheduler::execute(task);
	}
    cout << "start real-time Tread\n";
}
