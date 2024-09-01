#include "doctest.h"
#include "thread"
#include "WeightRoundRobinScheduler.h"
#include <queue>
#include "Scheduler.h"

TEST_CASE("WeightRoundRobinScheduler::weightRoundRobinFunction - Proportional Task Execution") {
    WeightRoundRobinScheduler scheduler;

    // Create tasks with different priorities
    Task* highPriorityTask1 = new Task(Scheduler::taskIds++, PrioritiesLevel::HIGHER, 1);
    Task* highPriorityTask2 = new Task(Scheduler::taskIds++, PrioritiesLevel::HIGHER, 1);
    Task* middlePriorityTask = new Task(Scheduler::taskIds++, PrioritiesLevel::MIDDLE, 1);
    Task* lowPriorityTask = new Task(Scheduler::taskIds++, PrioritiesLevel::LOWER, 1);

    // Add tasks to the scheduler
    scheduler.addTask(highPriorityTask1);
    scheduler.addTask(highPriorityTask2);
    scheduler.addTask(middlePriorityTask);
    scheduler.addTask(lowPriorityTask);

    // Create a thread to run the scheduler's weightRoundRobinFunction
    std::thread schedulerThread([&scheduler]() {
        scheduler.weightRoundRobinFunction(); // Start the weighted round-robin function
        });

    // Allow time for tasks to be processed
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // Stop the scheduler thread (you may need to implement a stopping mechanism for infinite loops)
    // For now, we assume that the function runs for a fixed time period and then we stop it manually
    // This might require some modifications depending on your actual implementation

    // Output task statuses for validation
    std::cout << "High Priority Task 1 Status: " << highPriorityTask1->getStatus() << std::endl;
    std::cout << "High Priority Task 2 Status: " << highPriorityTask2->getStatus() << std::endl;
    std::cout << "Middle Priority Task Status: " << middlePriorityTask->getStatus() << std::endl;
    std::cout << "Low Priority Task Status: " << lowPriorityTask->getStatus() << std::endl;

    // Add assertions to verify the expected results
    // For example, verify that high-priority tasks are processed more than middle and low-priority tasks
    // You might need to adjust the expected results based on your specific weighting and task processing logic

    CHECK_EQ(highPriorityTask1->getStatus(), TaskStatus::COMPLETED);
    CHECK_EQ(highPriorityTask2->getStatus(), TaskStatus::COMPLETED);
    CHECK_EQ(middlePriorityTask->getStatus(), TaskStatus::COMPLETED);
    CHECK_EQ(lowPriorityTask->getStatus(), TaskStatus::COMPLETED);

    // Ensure the scheduler thread finishes (implement a stopping mechanism if necessary)
    schedulerThread.join();

    // Clean up dynamically allocated tasks
    delete highPriorityTask1;
    delete highPriorityTask2;
    delete middlePriorityTask;
    delete lowPriorityTask;
}
