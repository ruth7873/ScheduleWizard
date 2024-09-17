#include "Helper.h"
void clearAll(Scheduler scheduler) {
	//clear heaps
	while (!scheduler.getDeadlineTaskManager().isEmpty()) {
		scheduler.getDeadlineTaskManager().deadlineMechanism();
	}
	while (!scheduler.getIterativeTaskHandler().isEmpty()) {
		scheduler.getIterativeTaskHandler().popIterativeTask();
	}

	while (!scheduler.getRealTimeScheduler().getRealTimeQueue().empty())
		scheduler.getRealTimeScheduler().getRealTimeQueue().pop();

	while (!scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.empty())
		scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.pop();

	while (!scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::MIDDLE].queue.empty())
		scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::MIDDLE].queue.pop();

	while (!scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::LOWER].queue.empty())
		scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::LOWER].queue.pop();

	//Clear the static variables before each test
	LongTaskHandler::setSumOfAllSeconds(0);
	LongTaskHandler::setNumOfSeconds(0);
	LongTaskHandler::setAverageLength(0.0);

	Scheduler::totalRunningTask = 0;
}