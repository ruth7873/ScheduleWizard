#include "RealTimeScheduler.h"

void RealTimeScheduler::realTimeSchedulerFunction() {
	while (true) {
		while (realTimeQueue.empty());
		Task* task = RealTimeScheduler::realTimeQueue.front();
		RealTimeScheduler::realTimeQueue.pop();
		Scheduler::execute(task);
	}
}