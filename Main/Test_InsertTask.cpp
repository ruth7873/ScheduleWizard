#include "doctest.h"
#include "Scheduler.h"

TEST_CASE("Test Scheduler::InsertTask") {
    Scheduler scheduler;

    //SUBCASE("Insert a critical task") {
    //    Task criticalTask(Scheduler::taskIds++, PrioritiesLevel::CRITICAL, 2);
    //    scheduler.insertTask(&criticalTask);

    //    CHECK_EQ(scheduler.getRealTimeScheduler().getRealTimeQueue().size(), 1); // Assuming realTimeScheduler has a method to get the task count
    //}

    SUBCASE("Insert a non-critical task") {
        Task nonCriticalTask(Scheduler::taskIds++, PrioritiesLevel::MIDDLE, 20);
        scheduler.insertTask(&nonCriticalTask);

        CHECK_EQ(scheduler.getWrrQueues().getWrrQueues()[PrioritiesLevel::MIDDLE].queue.size(), 1); // Assuming wrrQueues has a method to get the task count
    }

    //SUBCASE("Insert a null task") {
    //    try {
    //        Task* nullTask = nullptr;

    //        scheduler.insertTask(nullTask);
    //    }
    //    catch (const exception& e) {
    //        CHECK_EQ("Error: Invalid task input. Please try again.", e.what());

    //        CHECK_EQ(scheduler.getRealTimeScheduler().getRealTimeQueue().size(), 0); // Ensure no task is added to real-time scheduler
    //        CHECK_EQ(scheduler.getWrrQueues().getWrrQueues()[PrioritiesLevel::HIGHER].queue.size(), 0); // Ensure no task is added to HIGHER queue
    //        CHECK_EQ(scheduler.getWrrQueues().getWrrQueues()[PrioritiesLevel::MIDDLE].queue.size(), 0); // Ensure no task is added to MIDDLE queue
    //        CHECK_EQ(scheduler.getWrrQueues().getWrrQueues()[PrioritiesLevel::LOWER].queue.size(), 0); // Ensure no task is added to LOWER queue
    //    }
    //}
}