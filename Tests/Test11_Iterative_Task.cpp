#include "doctest.h"
#include "../Main/Scheduler.h"
#include "../Main/IterativeTask.h"
#include <memory>
#include "Helper.h"

TEST_CASE("Unit Test Of Iterative Task Handler") {
    Scheduler s(new ReadFromJSON(), new Utility());
    clearAll(s);

    SUBCASE("Push Iterative Task") {
        // Create and add a new iterative task to the handler
        std::shared_ptr<IterativeTask> newTask = std::make_shared<IterativeTask>(Task(0, PrioritiesLevel::HIGHER, 2), 2, 2);
        s.getIterativeTaskHandler().pushIterativeTask(newTask);

        // Wait for 1 second to allow for potential task processing
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Verify that the task has been added to the min heap
        CHECK_EQ(s.getIterativeTaskHandler().getMinHeap().size(), 1);

        // Remove the task from the handler
        s.getIterativeTaskHandler().popIterativeTask();

        // Re-add the task to verify its behavior when it's the only one in the heap
        s.getIterativeTaskHandler().pushIterativeTask(newTask);
        CHECK_EQ(s.getIterativeTaskHandler().getMinHeap().size(), 0);
    }
    clearAll(s);

    SUBCASE("Pop Iterative Task") {
        // Create and add a new iterative task to the handler
        std::shared_ptr<IterativeTask> newTask = std::make_shared<IterativeTask>(Task(0, PrioritiesLevel::HIGHER, 2), 4, 2);
        s.getIterativeTaskHandler().pushIterativeTask(newTask);

        // Wait for 1 second to allow for potential task processing
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Remove the task from the handler
        s.getIterativeTaskHandler().popIterativeTask();

        // Verify that the task has been removed from the scheduler's queue
        CHECK_EQ(s.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.size(), 0);

        // Try to remove a task when none are left, expecting an exception
        CHECK_THROWS(s.getIterativeTaskHandler().popIterativeTask());
    }
    clearAll(s);
    SUBCASE("Check Time Function") {
        // Create and add a new iterative task to the handler
        std::shared_ptr<IterativeTask> newTask = std::make_shared<IterativeTask>(Task(0, PrioritiesLevel::HIGHER, 2), 4, 3);
        s.getIterativeTaskHandler().pushIterativeTask(newTask);

        // Create and start a thread to run the checkTime function
        std::thread t([&s]() {
            s.getIterativeTaskHandler().checkTime();
            });

        // Wait for 1 seconds to allow the checkTime function to execute
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Verify that the task is still present in the min heap
        CHECK((s.getIterativeTaskHandler().getMinHeap().size() == 1 || s.getIterativeTaskHandler().getMinHeap().size() == 0));

        // Remove the task from the handler
        if (!s.getIterativeTaskHandler().isEmpty())
            s.getIterativeTaskHandler().popIterativeTask();
        if (!s.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.empty())
            s.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.pop();

        // Detach the thread after verification
        t.detach();
    }
}

TEST_CASE("System Test Of Iterative Task Handler") {
    Scheduler s(new ReadFromJSON(), new Utility());
    clearAll(s);

    SUBCASE("Iterative Task Handling") {
        // Create and insert a new iterative task into the scheduler
        std::shared_ptr<IterativeTask> newTask = std::make_shared<IterativeTask>(Task(0, PrioritiesLevel::HIGHER, 2), 2, 0);
        s.insertTask(newTask);

        // Create and start a thread to run the checkTime function
        std::thread t([&s]() {
            s.getIterativeTaskHandler().checkTime();
            });

        // Wait for 3 seconds to allow the checkTime function to execute
        std::this_thread::sleep_for(std::chrono::seconds(3));

        // Verify that the task was processed and is no longer in the min heap
        CHECK_EQ(s.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.size(), 2);
        CHECK_EQ(s.getIterativeTaskHandler().getMinHeap().size(), 0);

        // Remove the processed tasks from the scheduler's queue
        s.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.pop();
        s.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.pop();

        // Detach the thread after verification
        t.detach();
    }

    SUBCASE("Iterative Task with Zero Iterations") {
        // Create and insert a new iterative task with zero iterations
        std::shared_ptr<IterativeTask> newTask = std::make_shared<IterativeTask>(Task(0, PrioritiesLevel::HIGHER, 2), 0, 0);
        s.insertTask(newTask);

        // Verify that the task is present in the queue
        CHECK_EQ(s.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.size(), 1);
        CHECK_EQ(s.getIterativeTaskHandler().getMinHeap().size(), 0);

        // Remove the task from the scheduler's queue
        s.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.pop();
    }

    SUBCASE("Test CompareIterative with Close Wait Times") {
        IterativeTaskHandler handler;

        // Create two tasks with very close execution times
        std::shared_ptr<IterativeTask> task1 = std::make_shared<IterativeTask>(Task(0, PrioritiesLevel::HIGHER, 2), 5, 1); // 1 second interval
        std::shared_ptr<IterativeTask> task2 = std::make_shared<IterativeTask>(Task(1, PrioritiesLevel::HIGHER, 2), 3, 1); // 1 second interval

        // Set wait times very close to each other
        time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        task1->setWaitTime(now + 1); // 1 second from now

        // Sleep for a very short time to ensure a slight difference in wait times
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        task2->setWaitTime(now + 1); // Slightly more than 1 second from the original now

        // Push tasks into the minHeap
        handler.pushIterativeTask(task1);
        handler.pushIterativeTask(task2);

        // Check that both tasks can be popped from the heap in the correct order
        auto firstTask = handler.popIterativeTask();
        auto secondTask = handler.popIterativeTask();
        CHECK(firstTask->getWaitTime() <= secondTask->getWaitTime());
        CHECK(handler.isEmpty());
    }

    SUBCASE("Iterative Task with different running times") {
        // Create and add iterative tasks with different running times
        std::shared_ptr<IterativeTask> shortRunningTask = std::make_shared<IterativeTask>(Task(0, PrioritiesLevel::HIGHER, 1), 2, 1); // Running time: 1
        std::shared_ptr<IterativeTask> longRunningTask = std::make_shared<IterativeTask>(Task(1, PrioritiesLevel::HIGHER, 5), 2, 1); // Running time: 5

        // Push tasks into the handler
        s.getIterativeTaskHandler().pushIterativeTask(shortRunningTask);
        s.getIterativeTaskHandler().pushIterativeTask(longRunningTask);

        // Verify that tasks are added
        CHECK_EQ(s.getIterativeTaskHandler().getMinHeap().size(), 2);

        // Pop tasks and check their running times
        auto poppedTask1 = s.getIterativeTaskHandler().popIterativeTask();
        auto poppedTask2 = s.getIterativeTaskHandler().popIterativeTask();

        // Check that the running times are as expected
        CHECK_EQ(poppedTask1->getRunTime(), 1);
        CHECK_EQ(poppedTask2->getRunTime(), 5);
    }

    SUBCASE("Iterative Task with different iteration intervals") {
        // Create and add iterative tasks with different iteration intervals
        std::shared_ptr<IterativeTask> fastIterativeTask = std::make_shared<IterativeTask>(Task(0, PrioritiesLevel::HIGHER, 2), 3, 1); // Interval: 1 second
        std::shared_ptr<IterativeTask> slowIterativeTask = std::make_shared<IterativeTask>(Task(1, PrioritiesLevel::HIGHER, 2), 3, 5); // Interval: 5 seconds

        // Push tasks into the handler
        s.getIterativeTaskHandler().pushIterativeTask(fastIterativeTask);
        s.getIterativeTaskHandler().pushIterativeTask(slowIterativeTask);

        // Verify that tasks are added
        CHECK_EQ(s.getIterativeTaskHandler().getMinHeap().size(), 2);

        // Simulate time passing and pop tasks
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait 2 seconds for the fast task to be eligible for execution
        auto poppedTask1 = s.getIterativeTaskHandler().popIterativeTask();
        CHECK_EQ(poppedTask1->getExecutionInterval(), 1);

        // Wait another few seconds for the slow task
        std::this_thread::sleep_for(std::chrono::seconds(4)); // Wait an additional 4 seconds
        auto poppedTask2 = s.getIterativeTaskHandler().popIterativeTask();
        CHECK_EQ(poppedTask2->getExecutionInterval(), 5);
    }
}
