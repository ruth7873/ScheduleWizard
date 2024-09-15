#include "doctest.h"
#include <thread>
#include <chrono>
#include <memory>
#include "../Main/Scheduler.h"
#include "../Main/RealTimeScheduler.h"
//#include "Helper.h"

TEST_CASE("Test RealTimeScheduler::realTimeSchedulerFunction") {
	Scheduler scheduler;
	//clearAll(scheduler);

	SUBCASE("Successful Execution of Real-Time Tasks") {
		shared_ptr<Task> task1(new Task(Scheduler::taskIds++, PrioritiesLevel::CRITICAL, 2));

		scheduler.insertTask(task1);
		//scheduler.getRealTimeScheduler().realTimeSchedulerFunction();
		std::thread schedulerThread(&RealTimeScheduler::realTimeSchedulerFunction, &scheduler.getRealTimeScheduler());
		// Give some time for tasks to be executed
		schedulerThread.detach();  // We detach instead of join to avoid infinite loop
		std::this_thread::sleep_for(std::chrono::seconds(6));


		CHECK_EQ(scheduler.getRealTimeScheduler().getRealTimeQueue().size(), 0);
		CHECK_EQ(task1->getStatus(), TaskStatus::COMPLETED);
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
	SUBCASE("Test RealTimeScheduler Destructor") {
		{
			RealTimeScheduler rts;
			shared_ptr<Task> task1(new Task(1, PrioritiesLevel::CRITICAL, 2));
			shared_ptr<Task> task2(new Task(2, PrioritiesLevel::CRITICAL, 3));
			rts.addTask(task1);
			rts.addTask(task2);

			CHECK_EQ(rts.getRealTimeQueue().size(), 2);
		} // RealTimeScheduler goes out of scope here, destructor is called

		// We can't directly check the queue size after destruction,
		// but we can verify that the destructor didn't crash
		CHECK(true); // If we reach here, destructor completed without crashing
	}
}
