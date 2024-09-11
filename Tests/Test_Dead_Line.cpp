//#include "doctest.h"
//#include "DeadLineTask.h"
//#include "DeadlineTaskManager.h"
//#include "Scheduler.h"
//#include <chrono>
//#include <thread>
//#include <memory>
//
////TEST_CASE("Test DeadLineTask creation and management") {
////    DeadlineTaskManager manager;
////
////     Create a basic task
////    Task basicTask1(1, PrioritiesLevel::HIGHER, 10, TaskStatus::CREATION);
////    DeadLineTask deadlineTask1(basicTask1, time(nullptr) + 5); // Set deadline 5 seconds in the future
////
////     Check that the task is added to the manager
////     (You might need an accessor method or modify the manager class to check its state)
////
////    auto incomingTask = manager.getUpcomingTask(); // Hypothetical function to fetch the next upcoming task
////    CHECK(incomingTask->getId() == deadlineTask1.getId());
////}
//
//TEST_CASE("Test Deadline Mechanism") {
//    DeadlineTaskManager manager;
//    Scheduler scheduler;
//
//    auto Task1 = std::make_shared<Task>(1, PrioritiesLevel::HIGHER, 15);
//    auto baseTaskPtr = std::make_shared<DeadLineTask>(DeadLineTask(Task(2, PrioritiesLevel::LOWER, 10), time(nullptr) + 2));
//
//     //Insert 
//    scheduler.insertTask(Task1);
//    scheduler.insertTask(baseTaskPtr); // Assuming insertTask is adjusted to take Task
//
//    // Start the scheduler in a new thread
//    std::thread deadLineThread([&scheduler]() {
//        cout << scheduler.getRealTimeScheduler().getRealTimeQueue().size() << "----------------" << endl;
//        scheduler.getDeadlineTaskManager().deadlineMechanism();
//        });
//      //   Allow some time for tasks to be processed
//    std::this_thread::sleep_for(std::chrono::seconds(3));
//
//    // Terminate the thread by stopping its execution manually
//    deadLineThread.detach();  // Detach the thread to allow it to finish
//    cout << scheduler.getRealTimeScheduler().getRealTimeQueue().size() << "----------------" << endl;
//    // Assuming there's a way to check the task's priority
//    CHECK(scheduler.getRealTimeScheduler().getRealTimeQueue().size() == 1);
//}
//
////
////TEST_CASE("Test Task Retrieval Order") {
////    DeadlineTaskManager manager;
////
////    // Create several tasks with different deadlines
////    Task task3(3, PrioritiesLevel::MIDDLE, 1, TaskStatus::CREATION);
////    DeadLineTask deadlineTask3(task3, time(nullptr) + 3); // DeadLine in 3 seconds
////
////    Task task4(4, PrioritiesLevel::MIDDLE, 1, TaskStatus::CREATION);
////    DeadLineTask deadlineTask4(task4, time(nullptr) + 1); // DeadLine in 1 second
////
////    Task task5(5, PrioritiesLevel::MIDDLE, 1, TaskStatus::CREATION);
////    DeadLineTask deadlineTask5(task5, time(nullptr) + 2); // DeadLine in 2 seconds
////
////    // Now we add to manager
////    manager.addTask(std::make_shared<DeadLineTask>(deadlineTask3));
////    manager.addTask(std::make_shared<DeadLineTask>(deadlineTask4));
////    manager.addTask(std::make_shared<DeadLineTask>(deadlineTask5));
////
////    // Process the deadline mechanism
////    std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for the first task's deadline
////
////    manager.deadlineMechanism();  // Should promote the task with ID 4 to critical
////
////    // Check if the expected order is maintained
////    auto topTask = manager.getUpcomingTask(); // Get the next task in queue
////    CHECK(topTask->getId() == task4.getId()); // ID 4 should be top
////
////    // Continuing the process to check other priorities
////    std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for Task ID 3 deadline
////    manager.deadlineMechanism();  // Process deadline mechanism again
////
////    topTask = manager.getUpcomingTask(); // Check the next task again
////    CHECK(topTask->getId() == task5.getId()); // ID 5 should be next
////}