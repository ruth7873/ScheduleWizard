#include "doctest.h"
#include "Scheduler.h"
#include "IterativeTask.h"
#include <memory>

TEST_CASE("Unit Test Of Iterative Task Handler") {
    Scheduler s;

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

    SUBCASE("Check Time Function") {
        // Create and add a new iterative task to the handler
        std::shared_ptr<IterativeTask> newTask = std::make_shared<IterativeTask>(Task(0, PrioritiesLevel::HIGHER, 2), 4, 1);
        s.getIterativeTaskHandler().pushIterativeTask(newTask);

        // Create and start a thread to run the checkTime function
        std::thread t([&s]() {
            s.getIterativeTaskHandler().checkTime();
            });

        // Wait for 2 seconds to allow the checkTime function to execute
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // Verify that the task is still present in the min heap
        CHECK_EQ(s.getIterativeTaskHandler().getMinHeap().size(), 1);

        // Remove the task from the handler
        s.getIterativeTaskHandler().popIterativeTask();
        s.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.pop();

        // Detach the thread after verification
        t.detach();
    }
}

TEST_CASE("System Test Of Iterative Task Handler") {
    Scheduler s;

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
}