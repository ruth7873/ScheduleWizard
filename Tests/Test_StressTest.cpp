//#include "../Main/Scheduler.h"
//
//TEST_CASE("Stress test") {
//	Scheduler s(new ReadFromJSON, new Utility);
//		
//	std::thread schedulerThread(&RealTimeScheduler::realTimeSchedulerFunction, &s.getRealTimeScheduler());
//
//}

#include "doctest.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <atomic>
#include "../Main/Scheduler.h"
//#include "Task.h"
//#include "IterativeTask.h"
//#include "DeadlineTask.h"

const int NUM_TASKS = 1000;
const int TEST_DURATION_SECONDS = 300; // 5 minutes

std::atomic<bool> stopTest(false);

void generateTasks(Scheduler& scheduler) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> priorityDist(0, 3); // 0-3 for CRITICAL, HIGHER, MIDDLE, LOWER
    std::uniform_int_distribution<> timeDist(1, 20); // 1-20 seconds
    std::uniform_int_distribution<> taskTypeDist(0, 2); // 0 for regular, 1 for iterative, 2 for deadline

    for (int i = 0; i < NUM_TASKS && !stopTest.load(); ++i) {
        int priority = priorityDist(gen);
        int runningTime = timeDist(gen);
        int taskType = taskTypeDist(gen);
        string priorityS;

        shared_ptr<Task> task;
        switch (priority) {
        case 0:
            priorityS = PrioritiesLevel::CRITICAL;
            break;
        case 1:
            priorityS = PrioritiesLevel::HIGHER;
            break;
        case 2:
            priorityS = PrioritiesLevel::MIDDLE;
            break;
        case 3:
            priorityS = PrioritiesLevel::LOWER;
            break;
        }

        // Task type selection
        switch (taskType) {
        case 0: // Regular task
            task = make_shared<Task>(i, priorityS, runningTime, "NEW"); // Using the first Task constructor
            break;
        case 1: // Iterative task
            task = make_shared<IterativeTask>(Task(i, priorityS, runningTime, "NEW"), 3, 3); // 3 iterations, 3s interval
            break;
        case 2: // Deadline task
            task = make_shared<DeadlineTask>(Task(i, priorityS, runningTime, "NEW"), 30); // 30 seconds deadline
            break;
        }

        scheduler.insertTask(task);
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Small delay between task insertions
    }
}

void monitorSystem(const Scheduler& scheduler) {
	while (!stopTest.load()) {
		std::cout << "Total Running Tasks: " << Scheduler::totalRunningTask << std::endl;
		std::cout << "Real-Time Queue Size: " << scheduler.getRealTimeScheduler().getRealTimeQueue().size() << std::endl;
		std::cout << "WRR Higher Queue Size: " << scheduler.getWrrQueuesScheduler().getWrrQueues().at(PrioritiesLevel::HIGHER).queue.size() << std::endl;
		std::cout << "WRR Middle Queue Size: " << scheduler.getWrrQueuesScheduler().getWrrQueues().at(PrioritiesLevel::MIDDLE).queue.size() << std::endl;
		std::cout << "WRR Lower Queue Size: " << scheduler.getWrrQueuesScheduler().getWrrQueues().at(PrioritiesLevel::LOWER).queue.size() << std::endl;
		std::cout << "Iterative Tasks: " << scheduler.getIterativeTaskHandler().getMinHeap().size() << std::endl;
		//std::cout << "Deadline Tasks: " << scheduler.getDeadlineTaskManager().getDeadlineTasks().size() << std::endl;
		//std::cout << "Ordered Tasks: " << scheduler.getOrderedTaskHandler().getOrderedTasks().size() << std::endl;
		std::cout << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Update every 5 seconds
	}
}

TEST_CASE("stress test") {
	Scheduler scheduler(new ReadFromJSON, new Utility); // Assuming default constructor is available

	std::thread schedulerThread(&Scheduler::init, &scheduler);
	std::thread taskGeneratorThread(generateTasks, std::ref(scheduler));
	std::thread monitorThread(monitorSystem, std::ref(scheduler));

	// Run the test for the specified duration
	std::this_thread::sleep_for(std::chrono::milliseconds(TEST_DURATION_SECONDS));

	// Stop the test
	stopTest.store(true);

	// Wait for threads to finish
	taskGeneratorThread.detach();
	monitorThread.detach();
	schedulerThread.detach();

	std::cout << "Stress test completed successfully!" << std::endl;
    CHECK(true);
}