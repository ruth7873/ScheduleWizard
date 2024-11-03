//#include "doctest.h"
//#include <chrono>
//#include <thread>
//#include <memory>
//#include "../Main/WeightRoundRobinScheduler.h"
//#include "../Main/Scheduler.h"
//#include "Helper.h"
//using namespace std;
//TEST_CASE("Test Weighted Round Robin scheduler") {
//    Scheduler scheduler(new ReadFromJSON(), new Utility());
//    WeightRoundRobinScheduler WRRscheduler;
//    clearAll(scheduler);
//    SUBCASE("Insert tasks into the correct queue based on priority") {
//        shared_ptr<Task> higherTask(new Task(1, PrioritiesLevel::HIGHER, 5));
//        shared_ptr<Task> middleTask(new Task(2, PrioritiesLevel::MIDDLE, 10));
//        shared_ptr<Task> lowerTask(new Task(3, PrioritiesLevel::LOWER, 15));
//        scheduler.insertTask(higherTask);
//        scheduler.insertTask(middleTask);
//        scheduler.insertTask(lowerTask);
//        CHECK_EQ(scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.size(), 1);
//        CHECK_EQ(scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::MIDDLE].queue.size(), 1);
//        CHECK_EQ(scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::LOWER].queue.size(), 1);
//    }
//    SUBCASE("Test the running tasks is according to the Qs") {
//        clearAll(scheduler);
//        shared_ptr<Task> higherTask(new Task(1, PrioritiesLevel::HIGHER, 2));
//        shared_ptr<Task> middleTask(new Task(2, PrioritiesLevel::MIDDLE, 2));
//        shared_ptr<Task> lowerTask(new Task(3, PrioritiesLevel::LOWER, 2));
//        scheduler.insertTask(higherTask);
//        scheduler.insertTask(middleTask);
//        scheduler.insertTask(lowerTask);
//        // Start the scheduler in a new thread
//        std::thread WRRThread([&scheduler]() {
//            scheduler.getWrrQueuesScheduler().weightRoundRobinFunction();
//            });
//        // Allow some time for tasks to be processed
//        std::this_thread::sleep_for(std::chrono::milliseconds(6));
//        // Detach the thread to finish execution
//        WRRThread.detach();
//        // The first task suppose to be from the higher Q
//        cout << "the Higher Q size: " << scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.size() << endl;
//        CHECK_EQ(scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.size(), 0);
//        // Allow some time for tasks to be processed
//        std::this_thread::sleep_for(std::chrono::milliseconds(2));
//        // The second task suppose to be from the middle Q
//        cout << "the middle Q size: " << scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::MIDDLE].queue.size() << endl;
//        CHECK_EQ(scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::MIDDLE].queue.size(), 0);
//    }
//    SUBCASE("Test the weight calculation") {
//        clearAll(scheduler);
//        shared_ptr<Task> higherTask1(new Task(1, PrioritiesLevel::HIGHER, 1));
//        shared_ptr<Task> higherTask2(new Task(2, PrioritiesLevel::HIGHER, 1));
//        shared_ptr<Task> higherTask3(new Task(3, PrioritiesLevel::HIGHER, 1));
//        shared_ptr<Task> middleTask(new Task(4, PrioritiesLevel::MIDDLE, 1));
//        shared_ptr<Task> lowerTask1(new Task(5, PrioritiesLevel::LOWER, 1));
//        shared_ptr<Task> lowerTask2(new Task(6, PrioritiesLevel::LOWER, 1));
//        scheduler.insertTask(higherTask1);
//        scheduler.insertTask(higherTask2);
//        scheduler.insertTask(higherTask3);
//        scheduler.insertTask(middleTask);
//        scheduler.insertTask(lowerTask1);
//        scheduler.insertTask(lowerTask2);
//        // Start the scheduler in a new thread
//        std::thread WRRThread([&scheduler]() {
//            scheduler.getWrrQueuesScheduler().weightRoundRobinFunction();
//            });
//        // By the right calculating the weight of the higher Q = 3 tasks for executing
//        // let 3 tasks from the higher Q to run:
//        std::this_thread::sleep_for(std::chrono::milliseconds(4));
//        // Detach the thread to finish execution
//        WRRThread.detach();
//        // The higher Q suppose to be empty
//        cout << "the Higher Q size: " << scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.size() << endl;
//        CHECK_EQ(scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.size(), 0);
//        // Allow some time for tasks to be processed
//        std::this_thread::sleep_for(std::chrono::milliseconds(2));
//        //// The second task suppose to be from the middle Q
//        cout << "the middle Q size: " << scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::MIDDLE].queue.size() << endl;
//        CHECK_EQ(scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::MIDDLE].queue.size(), 0);
//    }
//    SUBCASE("Test task status change after execution") {
//        clearAll(scheduler);
//        shared_ptr<Task> higherTask(new Task(1, PrioritiesLevel::HIGHER, 1));
//        scheduler.insertTask(higherTask);
//        std::thread WRRThread([&scheduler]() {
//            scheduler.getWrrQueuesScheduler().weightRoundRobinFunction();
//            });
//        std::this_thread::sleep_for(std::chrono::milliseconds(5));
//        WRRThread.detach();
//        // Check that task status is updated after execution
//        CHECK_EQ(higherTask->getStatus(), TaskStatus::COMPLETED);
//    }
//    SUBCASE("Test dynamic task addition during execution") {
//        clearAll(scheduler);
//        shared_ptr<Task> higherTask(new Task(1, PrioritiesLevel::HIGHER, 1));
//        scheduler.insertTask(higherTask);
//        // Start the scheduler in a new thread
//        std::thread WRRThread([&scheduler]() {
//            scheduler.getWrrQueuesScheduler().weightRoundRobinFunction();
//            });
//        // Add new task while scheduler is running
//        std::this_thread::sleep_for(std::chrono::milliseconds(1));
//        WRRThread.detach();
//        
//        shared_ptr<Task> middleTask(new Task(2, PrioritiesLevel::MIDDLE, 1));
//        scheduler.insertTask(middleTask);
//        
//        std::this_thread::sleep_for(std::chrono::milliseconds(4));
//        // Check that the dynamically added task is executed
//        CHECK_EQ(middleTask->getStatus(), TaskStatus::COMPLETED);
//    }
//    SUBCASE("Execute Higher Tasks by their arrival order") {
//        clearAll(scheduler);
//        shared_ptr<Task> task1(new Task(Scheduler::taskIds++, PrioritiesLevel::HIGHER, 2));
//        shared_ptr<Task> task2(new Task(Scheduler::taskIds++, PrioritiesLevel::HIGHER, 3));
//        shared_ptr<Task> task3(new Task(Scheduler::taskIds++, PrioritiesLevel::HIGHER, 5));
//
//        scheduler.insertTask(task1);
//        scheduler.insertTask(task2);
//        scheduler.insertTask(task3);
//
//        Scheduler::totalRunningTask = 1;//ensure that the task won't suspended because the long.
//        std::thread schedulerThread(&RealTimeScheduler::realTimeSchedulerFunction, &scheduler.getRealTimeScheduler());
//        // Give some time for tasks to be executed
//        schedulerThread.detach();  // We detach instead of join to avoid infinite loop
//        std::this_thread::sleep_for(std::chrono::milliseconds(3));
//        CHECK(task1->getStatus() == TaskStatus::COMPLETED);
//        CHECK(task2->getStatus() != TaskStatus::COMPLETED);
//        CHECK(task3->getStatus() != TaskStatus::COMPLETED);
//
//        std::this_thread::sleep_for(std::chrono::milliseconds(3));
//        CHECK(task2->getStatus() == TaskStatus::COMPLETED);
//        CHECK(task3->getStatus() != TaskStatus::COMPLETED);
//
//        std::this_thread::sleep_for(std::chrono::milliseconds(5));
//        CHECK(task3->getStatus() == TaskStatus::COMPLETED);
//
//    }
//
//    SUBCASE("Test WRR Destructor") {
//        {
//            WeightRoundRobinScheduler wrr;
//            shared_ptr<Task> task1(new Task(1, PrioritiesLevel::LOWER, 2));
//            shared_ptr<Task> task2(new Task(2, PrioritiesLevel::LOWER, 3));
//            wrr.addTask(task1);
//            wrr.addTask(task2);
//            CHECK_EQ(wrr.getWrrQueues()[PrioritiesLevel::LOWER].queue.size(), 2);
//        } // WeightRoundRobinScheduler goes out of scope here, destructor is called
//        // We can't directly check the queue size after destruction,
//        // but we can verify that the destructor didn't crash
//        CHECK(true); // If we reach here, destructor completed without crashing
//    }
//}