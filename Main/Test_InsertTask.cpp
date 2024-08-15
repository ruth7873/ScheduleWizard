#include "doctest.h"
#include "Scheduler.h"

TEST_CASE("Test Scheduler::InsertTask") {
    Scheduler scheduler;

    SUBCASE("Insert a critical task") {
        Task criticalTask(1, Consts::CRITICAL, 10, "Creation");
        scheduler.InsertTask(&criticalTask);

        CHECK_EQ(scheduler.getRealTimeScheduler().getRealTimeQueue().size(), 1); // Assuming realTimeScheduler has a method to get the task count
    }

    SUBCASE("Insert a non-critical task") {
        Task nonCriticalTask(2, Consts::MIDDLE, 20, "Creation");
        scheduler.InsertTask(&nonCriticalTask);

        CHECK_EQ(scheduler.getWrrQueues().getWrrQueues()[Consts::MIDDLE].queue.size(), 1); // Assuming wrrQueues has a method to get the task count
    }

    SUBCASE("Insert a null task") {
        try {
            Task* nullTask = nullptr;

            scheduler.InsertTask(nullTask);
        }
        catch (const exception& e) {
            CHECK_EQ("Error: Invalid task input. Please try again.", e.what());

            CHECK_EQ(scheduler.getRealTimeScheduler().getRealTimeQueue().size(), 0); // Ensure no task is added to real-time scheduler
            CHECK_EQ(scheduler.getWrrQueues().getWrrQueues()[Consts::HIGHER].queue.size(), 0); // Ensure no task is added to HIGHER queue
            CHECK_EQ(scheduler.getWrrQueues().getWrrQueues()[Consts::MIDDLE].queue.size(), 0); // Ensure no task is added to MIDDLE queue
            CHECK_EQ(scheduler.getWrrQueues().getWrrQueues()[Consts::LOWER].queue.size(), 0); // Ensure no task is added to LOWER queue
        }
    }
}
