#include "doctest.h"
#include "../Main/Scheduler.h"

TEST_CASE("Scheduler::execute") {
	Scheduler scheduler;
	SUBCASE("Scheduler::execute - Successful Execution of Critical Task") {
		shared_ptr<Task> criticalTask(new Task(Scheduler::taskIds++, PrioritiesLevel::CRITICAL, 2));

		scheduler.insertTask(criticalTask);

		std::thread schedulerThread(&RealTimeScheduler::realTimeSchedulerFunction, &scheduler.getRealTimeScheduler());
		std::this_thread::sleep_for(std::chrono::seconds(3));

		CHECK_EQ(criticalTask->getStatus(), TaskStatus::COMPLETED);

		schedulerThread.detach();
	}
	SUBCASE("Scheduler::execute - Preemption of Non-Critical Task") {
		shared_ptr<Task> higherPriorityTask(new Task(Scheduler::taskIds++, PrioritiesLevel::HIGHER, 5));
		shared_ptr<Task> criticalTask(new Task(Scheduler::taskIds++, PrioritiesLevel::CRITICAL, 2));

		// Insert tasks into the scheduler
		scheduler.insertTask(higherPriorityTask);
		scheduler.insertTask(criticalTask);

		// Create a thread to execute tasks in the scheduler
		std::thread schedulerThread([&scheduler, higherPriorityTask, criticalTask]() {
			// Simulate execution of tasks
			scheduler.execute(higherPriorityTask); // Start executing the non-critical task
			scheduler.execute(criticalTask);       // Then, execute the critical task
			});

		// Allow time for tasks to be processed
		std::this_thread::sleep_for(std::chrono::seconds(5));

		// Check if the higher priority task was preempted and critical task completed
		CHECK_EQ(higherPriorityTask->getStatus(), TaskStatus::SUSPENDED);
		CHECK_EQ(criticalTask->getStatus(), TaskStatus::COMPLETED);

		// Ensure the scheduler thread finishes
		schedulerThread.join();
	}

	SUBCASE("Scheduler::execute - Boundary Condition: Minimum Running Time") {
		shared_ptr<Task> minTimeTask(new Task(Scheduler::taskIds++, PrioritiesLevel::CRITICAL, 0));

		scheduler.insertTask(minTimeTask);

		std::thread schedulerThread(&RealTimeScheduler::realTimeSchedulerFunction, &scheduler.getRealTimeScheduler());
		std::this_thread::sleep_for(std::chrono::seconds(1));

		CHECK_EQ(minTimeTask->getStatus(), TaskStatus::COMPLETED);

		schedulerThread.detach();
	}
	SUBCASE("Scheduler::execute - Boundary Condition: Maximum Running Time") {
		// Assuming a reasonable maximum running time, like 100 for example
		shared_ptr<Task> maxTimeTask(new Task(Scheduler::taskIds++, PrioritiesLevel::CRITICAL, 10));

		scheduler.insertTask(maxTimeTask);

		std::thread schedulerThread(&RealTimeScheduler::realTimeSchedulerFunction, &scheduler.getRealTimeScheduler());
		std::this_thread::sleep_for(std::chrono::seconds(15)); // Wait until task is completed

		CHECK_EQ(maxTimeTask->getStatus(), TaskStatus::COMPLETED);

		schedulerThread.detach();
	}
}