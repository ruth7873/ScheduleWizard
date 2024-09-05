#include "doctest.h"
#include <memory>
#include "Scheduler.h"

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
        try {
            shared_ptr<Task> nullTask = nullptr;

            scheduler.insertTask(nullTask);
        }
        catch (const exception& e) {
            CHECK_EQ("Error: Invalid task input. Please try again.", e.what());

            CHECK_EQ(scheduler.getRealTimeScheduler().getRealTimeQueue().size(), 0); // Ensure no task is added to real-time scheduler
            CHECK_EQ(scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.size(), 0); // Ensure no task is added to HIGHER queue
            CHECK_EQ(scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::MIDDLE].queue.size(), 0); // Ensure no task is added to MIDDLE queue
            CHECK_EQ(scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::LOWER].queue.size(), 0); // Ensure no task is added to LOWER queue
        }
    }
}