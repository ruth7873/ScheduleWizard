#include "Helper.h"
void PopAllTheQueue(Scheduler scheduler) {

	while (!scheduler.getRealTimeScheduler().getRealTimeQueue().empty())
		scheduler.getRealTimeScheduler().getRealTimeQueue().pop();

	while (!scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.empty())
		scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.pop();

	while (!scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::MIDDLE].queue.empty())
		scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::MIDDLE].queue.pop();

	while (!scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::LOWER].queue.empty())

		scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::LOWER].queue.pop();
}