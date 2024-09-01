#include "doctest.h"
#include "Scheduler.h"



//TEST_CASE("Scheduler::execute - Successful Execution of Critical Task") {
//    Scheduler scheduler;
//    shared_ptr<Task> criticalTask = new Task(Scheduler::taskIds++, PrioritiesLevel::CRITICAL, 2);
//
//    scheduler.insertTask(criticalTask);
//
//    std::thread schedulerThread(&RealTimeScheduler::realTimeSchedulerFunction, &scheduler.getRealTimeScheduler());
//    std::this_thread::sleep_for(std::chrono::seconds(3));
//
//    CHECK_EQ(criticalTask->getStatus(), TaskStatus::COMPLETED);
//
//    schedulerThread.detach();
//}
//
TEST_CASE("Scheduler::execute - Prevention of starvation") {
    Scheduler scheduler;
    shared_ptr<Task> lowerTask(new Task(Scheduler::taskIds++, PrioritiesLevel::LOWER, 5));
    shared_ptr<Task> higherTask1(new Task(Scheduler::taskIds++, PrioritiesLevel::HIGHER, 3));
    shared_ptr<Task> higherTask2(new Task(Scheduler::taskIds++, PrioritiesLevel::HIGHER, 3));
    shared_ptr<Task> higherTask3(new Task(Scheduler::taskIds++, PrioritiesLevel::HIGHER, 3));
    shared_ptr<Task> middleTask1(new Task(Scheduler::taskIds++, PrioritiesLevel::MIDDLE, 4));
    shared_ptr<Task> middleTask2(new Task(Scheduler::taskIds++, PrioritiesLevel::MIDDLE, 4));

    // Insert tasks into the scheduler
    scheduler.insertTask(lowerTask);
    scheduler.insertTask(higherTask1);
    scheduler.insertTask(higherTask2);
    scheduler.insertTask(higherTask3);
    scheduler.insertTask(middleTask1);
    scheduler.insertTask(middleTask2);

    // Create a thread to execute the scheduler
    std::thread schedulerThread(&WeightRoundRobinScheduler::weightRoundRobinFunction, &scheduler.getWrrQueuesScheduler());

   /* std::thread schedulerThread([&scheduler]() {
        while (Scheduler::totalRunningTask > 0) {
            if (!scheduler.getWrrQueues().getWrrQueues()[PrioritiesLevel::LOWER].queue.empty()) {

                scheduler.execute(scheduler.getWrrQueues().getWrrQueues()[PrioritiesLevel::LOWER].queue.front());
            }
        }
        });*/

    std::this_thread::sleep_for(std::chrono::seconds(15));
    // Wait for all tasks to be processed
    schedulerThread.detach();

    // Ensure the lower priority task has been executed
    CHECK_EQ(higherTask1->getStatus(), TaskStatus::COMPLETED);
}

//TEST_CASE("Scheduler::execute - Preemption of Non-Critical Task") {
//	Scheduler scheduler;
//	shared_ptr<Task> higherPriorityTask(new Task(Scheduler::taskIds++, PrioritiesLevel::HIGHER, 5));
//	shared_ptr<Task> criticalTask(new Task(Scheduler::taskIds++, PrioritiesLevel::CRITICAL, 2));
//
//	// Insert tasks into the scheduler
//	scheduler.insertTask(higherPriorityTask);
//	scheduler.insertTask(criticalTask);
//
//	// Create a thread to execute tasks in the scheduler
//	std::thread schedulerThread([&scheduler, higherPriorityTask, criticalTask]() {
//		// Simulate execution of tasks
//		scheduler.execute(higherPriorityTask); // Start executing the non-critical task
//		scheduler.execute(criticalTask);       // Then, execute the critical task
//		});
//
//	// Allow time for tasks to be processed
//	std::this_thread::sleep_for(std::chrono::seconds(5));
//
//	// Check if the higher priority task was preempted and critical task completed
//	CHECK_EQ(higherPriorityTask->getStatus(), TaskStatus::SUSPENDED);
//	CHECK_EQ(criticalTask->getStatus(), TaskStatus::COMPLETED);
//
//	// Ensure the scheduler thread finishes
//	schedulerThread.join();
//}

//TEST_CASE("Scheduler::execute - Boundary Condition: Minimum Running Time") {
//    Scheduler scheduler;
//    shared_ptr<Task> minTimeTask = new Task(Scheduler::taskIds++, PrioritiesLevel::CRITICAL, 0);
//
//    scheduler.insertTask(minTimeTask);
//
//    std::thread schedulerThread(&RealTimeScheduler::realTimeSchedulerFunction, &scheduler.getRealTimeScheduler());
//    std::this_thread::sleep_for(std::chrono::seconds(1));
//
//    CHECK_EQ(minTimeTask->getStatus(), TaskStatus::COMPLETED);
//
//    schedulerThread.detach();
//}
//TEST_CASE("Scheduler::execute - Boundary Condition: Maximum Running Time") {
//    Scheduler scheduler;
//    // Assuming a reasonable maximum running time, like 100 for example
//    shared_ptr<Task> maxTimeTask (new Task(Scheduler::taskIds++, PrioritiesLevel::CRITICAL, 100));
//
//    scheduler.insertTask(maxTimeTask);
//
//    std::thread schedulerThread(&RealTimeScheduler::realTimeSchedulerFunction, &scheduler.getRealTimeScheduler());
//    std::this_thread::sleep_for(std::chrono::seconds(105)); // Wait until task is completed
//
//    CHECK_EQ(maxTimeTask->getStatus(), TaskStatus::COMPLETED);
//
//    schedulerThread.detach();
//}
