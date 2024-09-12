#include "doctest.h"
#include "../Main/DeadLineTask.h"
#include "../Main/DeadlineTaskManager.h"
#include "../Main/Scheduler.h"
#include <chrono>
#include <thread>
#include <memory>

using namespace std;

TEST_CASE("Test DeadLineTask creation and management") {

    // Initialize DeadlineTaskManager and Scheduler
    DeadlineTaskManager manager;
    Scheduler scheduler;

    //// Test the insertion of tasks into the heap
    //SUBCASE("Test deadline task inserts to the heap") {
    //    // Create tasks (task1: regular task, task2: deadline task)
    //    auto task1 = std::make_shared<Task>(1, PrioritiesLevel::HIGHER, 15);
    //    auto task2 = std::make_shared<DeadLineTask>(DeadLineTask(Task(2, PrioritiesLevel::LOWER, 10), time(nullptr) + 2));

    //    // Insert tasks into the scheduler
    //    scheduler.insertTask(task1);
    //    scheduler.insertTask(task2);  // Assuming insertTask accepts both Task and DeadLineTask

    //    // Verify if the correct task is at the top of the heap
    //    auto incomingTask = manager.getUpcomingTask();
    //    CHECK(incomingTask->getId() == task2->getId());
    //}

    //// Test that a deadline task becomes critical after a certain time
    //SUBCASE("Test deadline time becomes critical") {
    //    auto Task1 = std::make_shared<Task>(1, PrioritiesLevel::HIGHER, 15);
    //    auto baseTaskPtr = std::make_shared<DeadLineTask>(DeadLineTask(Task(2, PrioritiesLevel::LOWER, 10), time(nullptr) + 2));

    //    // Insert tasks into the scheduler
    //    scheduler.insertTask(Task1);
    //    scheduler.insertTask(baseTaskPtr);

    //    // Run the deadline mechanism in a separate thread
    //    std::thread deadLineThread([&scheduler]() {
    //        cout << scheduler.getRealTimeScheduler().getRealTimeQueue().size() << "----------------" << endl;
    //        scheduler.getDeadlineTaskManager().deadlineMechanism();
    //        });

    //    // Allow time for tasks to be processed
    //    std::this_thread::sleep_for(std::chrono::seconds(3));

    //    // Detach the thread to finish execution
    //    deadLineThread.detach();
    //    cout << scheduler.getRealTimeScheduler().getRealTimeQueue().size() << "----------------" << endl;

    //    // Verify that the task count in the real-time queue is 1
    //    CHECK(scheduler.getRealTimeScheduler().getRealTimeQueue().size() == 1);
    //}

    //// Test the build of the heap by the pop
    //SUBCASE("Test inserting tasks with different deadlines") {
    //    // Create deadline tasks with different deadlines
    //    auto deadlineTask1 = std::make_shared<DeadLineTask>(DeadLineTask(Task(1, PrioritiesLevel::LOWER, 1), time(nullptr) + 10));
    //    auto deadlineTask2 = std::make_shared<DeadLineTask>(DeadLineTask(Task(2, PrioritiesLevel::LOWER, 1), time(nullptr) + 1));

    //    // Insert tasks into the scheduler
    //    scheduler.insertTask(deadlineTask1);
    //    scheduler.insertTask(deadlineTask2);

    //    // Start the deadline mechanism in a separate thread
    //    std::thread deadLineThread([&scheduler]() {
    //        scheduler.getDeadlineTaskManager().deadlineMechanism();
    //        });

    //    // Detach the thread to finish execution
    //    deadLineThread.detach();

    //    // Check the task at the top of the heap
    //    auto topTask = manager.getUpcomingTask();
    //    CHECK_EQ(topTask->getId(), 2);  // Task with ID 2 should be at the top

    //    // Wait for the first task's deadline to pass
    //    std::this_thread::sleep_for(std::chrono::seconds(2));

    //    // Check the task at the top again
    //    topTask = manager.getUpcomingTask();
    //    CHECK_EQ(topTask->getId(), 1);  // Task with ID 1 should now be at the top
    //}

    SUBCASE("Test task removal after completion or critical status") {
        try {
            //auto deadlineTask = std::make_shared<DeadLineTask>(Task(1, PrioritiesLevel::LOWER, 10), time(nullptr) + 2);
            //// Insert tasks into the scheduler
            //scheduler.insertTask(deadlineTask);

            //// Start the deadline mechanism in a separate thread
            //std::thread deadLineThread([&scheduler]() {
            //    scheduler.getDeadlineTaskManager().deadlineMechanism();
            //    });

            //// Detach the thread to finish execution
            //deadLineThread.detach();

            //// Wait for the first task's deadline to pass
            //std::this_thread::sleep_for(std::chrono::seconds(2));
            scheduler.getDeadlineTaskManager().getUpcomingTask();
        }
        catch (exception e) {
            CHECK_EQ(e.what(), "Heap is empty");  // No task should be left in the heap

        }


    }
}


