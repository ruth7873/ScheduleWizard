#include "doctest.h"
#include "RealTimeScheduler.h"
#include "Scheduler.h"
#include <thread>
#include <chrono>
#include <memory>

TEST_CASE("Test RealTimeScheduler::realTimeSchedulerFunction") {
	Scheduler scheduler;

	SUBCASE("Successful Execution of Real-Time Tasks") {
		shared_ptr<Task> task1(new Task(Scheduler::taskIds++, PrioritiesLevel::CRITICAL, 2));
		shared_ptr<Task> task2(new Task(Scheduler::taskIds++, PrioritiesLevel::CRITICAL, 3));

		scheduler.insertTask(task1);
		scheduler.insertTask(task2);
		//scheduler.getRealTimeScheduler().realTimeSchedulerFunction();
		std::thread schedulerThread(&RealTimeScheduler::realTimeSchedulerFunction, &scheduler.getRealTimeScheduler());
		// Give some time for tasks to be executed
		std::this_thread::sleep_for(std::chrono::seconds(15));

		schedulerThread.detach();  // We detach instead of join to avoid infinite loop

		CHECK_EQ(scheduler.getRealTimeScheduler().getRealTimeQueue().size(), 0);
		CHECK_EQ(task1->getStatus(), TaskStatus::COMPLETED);
		CHECK_EQ(task2->getStatus(), TaskStatus::COMPLETED);
		// Here you might want to add more checks to verify task execution
	}

	SUBCASE("Empty Real-Time Queue - Blocking Behavior") {
		// Start the real-time scheduler in a separate thread
		std::thread schedulerThread([&scheduler]() {
			scheduler.getRealTimeScheduler().realTimeSchedulerFunction();
			});

		// Give some time for the scheduler to attempt to execute tasks (should be blocking on an empty queue)
		std::this_thread::sleep_for(std::chrono::seconds(3));

		// Since no tasks are added, the queue should remain empty
		CHECK_EQ(scheduler.getRealTimeScheduler().getRealTimeQueue().size(), 0);

		// Detach the thread to prevent blocking the test case
		schedulerThread.detach();
	}
}