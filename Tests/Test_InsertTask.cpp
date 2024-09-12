#include "doctest.h"
#include <memory>
#include "../Main/Scheduler.h"
#include "Helper.h"

TEST_CASE("Test Scheduler::InsertTask") {
    Scheduler scheduler;

    SUBCASE("Insert a critical task") {
        shared_ptr<Task> criticalTask(new Task(Scheduler::taskIds++, PrioritiesLevel::CRITICAL, 5));
        scheduler.insertTask(criticalTask);

        CHECK_EQ(scheduler.getRealTimeScheduler().getRealTimeQueue().size(), 1); // Assuming realTimeScheduler has a method to get the task count

        //scheduler.getRealTimeScheduler().getRealTimeQueue().pop();
    }

    SUBCASE("Insert a non-critical task") {
        shared_ptr<Task> nonCriticalTask(new Task(Scheduler::taskIds++, PrioritiesLevel::LOWER, 5));
        scheduler.insertTask(nonCriticalTask);

        CHECK_EQ(scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::LOWER].queue.size(), 1); // Assuming wrrQueues has a method to get the task count

        scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::LOWER].queue.pop();
    }
    SUBCASE("Insert a null task") {
        int realTimeQueueSize = scheduler.getRealTimeScheduler().getRealTimeQueue().size();
        int higherQueueSize = scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.size();
        int midddleQueueSize = scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::MIDDLE].queue.size();
        int lowerQueueSize = scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::LOWER].queue.size();

        int prevTotalRuningTask = Scheduler::totalRunningTask;

        shared_ptr<Task> nullTask = nullptr;

        scheduler.insertTask(nullTask);

        // Check that no tasks were added
        CHECK_EQ(scheduler.getRealTimeScheduler().getRealTimeQueue().size(), realTimeQueueSize);
        CHECK_EQ(scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.size(), higherQueueSize);
        CHECK_EQ(scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::MIDDLE].queue.size(), midddleQueueSize);
        CHECK_EQ(scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::LOWER].queue.size(), lowerQueueSize);

        // You might also want to check that totalRunningTask wasn't incremented
        // This assumes you have a method to get totalRunningTask
        CHECK_EQ(Scheduler::totalRunningTask, prevTotalRuningTask);
    }
    PopAllTheQueue(scheduler);

}