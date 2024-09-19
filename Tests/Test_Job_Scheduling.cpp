#include "doctest.h"
#include "fakeit.hpp"
#include "../Main/ReadFromJSON.h"
#include "../Main/Task.h"
#include "../Main/Scheduler.h"
#include <iostream>
#include <sstream>

using namespace std;
using namespace fakeit;

TEST_CASE("1. Job Scheduling Test") {
    // Mock the ReadFromJSON class
    Mock<ReadFromJSON> mockReadFromJSON;
    Mock<Utility> mockUtility;

    // Specify the lambda function's type explicitly
    auto createTasksLambda = [&](const std::string& filePath) -> void {};
    auto insertTaskFromInputLambda = [&]() -> void {};

    When(Method(mockReadFromJSON, createTasksFromJSON)).AlwaysDo(createTasksLambda);
    When(Method(mockUtility, insertTaskFromInput)).AlwaysDo(insertTaskFromInputLambda);

    // Replace the actual ReadFromJSON object and Utility object in the Scheduler with the mock objects
    ReadFromJSON& readFromJSONMock = mockReadFromJSON.get();
    Utility& utilityMock = mockUtility.get();

    // Set the mock objects in the Scheduler
    Scheduler s(&readFromJSONMock, &utilityMock);

    // Call the function
    std::thread Init_Scheduler_Thread([&s]() {
        SetThreadDescription(GetCurrentThread(), L"Init Scheduler");
        spdlog::info(Logger::LoggerInfo::START_READ_FROM_JSON_THREAD);
        CHECK_THROWS(s.init());
        });

    //TODO: add verify

    SUBCASE("1.1 Real-Time Task Submission") {
        shared_ptr<Task> criticalTask(new Task(Scheduler::taskIds++, PrioritiesLevel::CRITICAL, 5));
        shared_ptr<Task> nonCriticalTask(new Task(Scheduler::taskIds++, PrioritiesLevel::HIGHER, 5));

        s.insertTask(nonCriticalTask);
        s.insertTask(criticalTask);

        std::this_thread::sleep_for(std::chrono::milliseconds(3));

        CHECK_EQ(criticalTask->getStatus(), TaskStatus::RUNNING);
        CHECK((nonCriticalTask->getStatus() == TaskStatus::CREATION || nonCriticalTask->getStatus() == TaskStatus::SUSPENDED));

        std::this_thread::sleep_for(std::chrono::milliseconds(90));

        CHECK_EQ(criticalTask->getStatus(), TaskStatus::COMPLETED);
        CHECK_EQ(nonCriticalTask->getStatus(), TaskStatus::RUNNING);

    }

    SUBCASE("1.2 Non-Real-Time Task Submission") {
        shared_ptr<Task> higherTask1(new Task(Scheduler::taskIds++, PrioritiesLevel::HIGHER, 5));
        shared_ptr<Task> higherTask2(new Task(Scheduler::taskIds++, PrioritiesLevel::HIGHER, 1));
        shared_ptr<Task> higherTask3(new Task(Scheduler::taskIds++, PrioritiesLevel::HIGHER, 1));
        shared_ptr<Task> higherTask4(new Task(Scheduler::taskIds++, PrioritiesLevel::HIGHER, 1));
        shared_ptr<Task> lowerTask(new Task(Scheduler::taskIds++, PrioritiesLevel::LOWER, 5));

        s.insertTask(higherTask1);
        s.insertTask(higherTask2);
        s.insertTask(higherTask3);
        s.insertTask(higherTask4);
        s.insertTask(lowerTask);

        /*
        waiting 6 seconds bcs. wait that task also insert to Q's:
            1-2 sec - insert tasks.
            3-6 sec - wait that with certainty 'higherTask1' running.
        */ 
        
        std::this_thread::sleep_for(std::chrono::milliseconds(150));

        CHECK((higherTask1->getStatus() == TaskStatus::RUNNING || higherTask1->getStatus() == TaskStatus::SUSPENDED));

        //waiting 8 seconds bcs. wait that higher Q run 2 (5/2 = 5: tasks, 2:weight of higher Q) tasks 
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        CHECK((lowerTask->getStatus() == TaskStatus::RUNNING || lowerTask->getStatus() == TaskStatus::COMPLETED));
        CHECK_EQ(s.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.size(), 1);

        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }

    SUBCASE("1.3 Scheduling Mechanisms") {

        SUBCASE("1.3.1 Mix of Real-Time and Non-Real-Time Tasks Simultaneously") {
            shared_ptr<Task> criticalTask1(new Task(Scheduler::taskIds++, PrioritiesLevel::CRITICAL, 4));
            shared_ptr<Task> criticalTask2(new Task(Scheduler::taskIds++, PrioritiesLevel::CRITICAL, 2));
            shared_ptr<Task> higherTask(new Task(Scheduler::taskIds++, PrioritiesLevel::HIGHER, 2));
            shared_ptr<Task> middleTask(new Task(Scheduler::taskIds++, PrioritiesLevel::MIDDLE, 2));
            shared_ptr<Task> lowerTask(new Task(Scheduler::taskIds++, PrioritiesLevel::LOWER, 2));

            s.insertTask(lowerTask);
            s.insertTask(middleTask);
            s.insertTask(higherTask);
            s.insertTask(criticalTask1);
            s.insertTask(criticalTask2);

            // Wait that with certainty 'criticalTask1' running 
            std::this_thread::sleep_for(std::chrono::milliseconds(12));

            CHECK(criticalTask1->getStatus() == TaskStatus::RUNNING);
            CHECK(criticalTask2->getStatus() == TaskStatus::CREATION);

            // Checks that all non-critical tasks are created or suspended. They can be suspended 
            // if the WRR scheduler thread was started first, and started running a task from some Q,
            // but was suspended when the critical Q starts running.
            CHECK((higherTask->getStatus() == TaskStatus::CREATION || higherTask->getStatus() == TaskStatus::SUSPENDED));
            CHECK((middleTask->getStatus() == TaskStatus::CREATION || middleTask->getStatus() == TaskStatus::SUSPENDED));
            CHECK((lowerTask->getStatus() == TaskStatus::CREATION || lowerTask->getStatus() == TaskStatus::SUSPENDED));

            std::this_thread::sleep_for(std::chrono::milliseconds(150));

            CHECK_EQ(criticalTask1->getStatus(), TaskStatus::COMPLETED);
            CHECK_EQ(criticalTask2->getStatus(), TaskStatus::COMPLETED);

            // Sure higherTask is running, but it might be suspended because the average was lower than its running time.
            CHECK((higherTask->getStatus() == TaskStatus::RUNNING ||
                higherTask->getStatus() == TaskStatus::COMPLETED ||
                higherTask->getStatus() == TaskStatus::SUSPENDED));

            CHECK((middleTask->getStatus() == TaskStatus::RUNNING ||
                middleTask->getStatus() == TaskStatus::COMPLETED ||
                middleTask->getStatus() == TaskStatus::SUSPENDED));
            CHECK((lowerTask->getStatus() == TaskStatus::RUNNING ||
                lowerTask->getStatus() == TaskStatus::COMPLETED ||
                lowerTask->getStatus() == TaskStatus::SUSPENDED));

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } 
        
        SUBCASE("1.3.2 Mix of Base Task Type and Iterative Tasks Simultaneously") {
    //        shared_ptr<Task> criticalTask(new Task(Scheduler::taskIds++, PrioritiesLevel::CRITICAL, 5));
    //        shared_ptr<Task> higherTask(new Task(Scheduler::taskIds++, PrioritiesLevel::HIGHER, 2));
    //        shared_ptr<IterativeTask> iterativeTask1(new IterativeTask(Task(Scheduler::taskIds++, PrioritiesLevel::MIDDLE, 4), 3, 5));
    //        shared_ptr<IterativeTask> iterativeTask2(new IterativeTask(Task(Scheduler::taskIds++, PrioritiesLevel::LOWER, 4), 2, 4));

    //        s.insertTask(higherTask);
    //        s.insertTask(criticalTask);
    //        s.insertTask(iterativeTask1);
    //        s.insertTask(iterativeTask2);

    //        std::this_thread::sleep_for(std::chrono::milliseconds(3));

    //        CHECK_EQ(s.getIterativeTaskHandler().getMinHeap().size(), 2);
    //        // Testing that 'iterativeTask2' at the top of the heap, because its waiting time is lower than 'itetiveTask1'
    //        CHECK_EQ(s.getIterativeTaskHandler().getMinHeap().top(), iterativeTask2);
    //        CHECK_EQ(criticalTask->getStatus(), TaskStatus::RUNNING);

    //        std::this_thread::sleep_for(std::chrono::milliseconds(6));

    //        // The Heap size should be set to 1. Since the 'iterativeTask2' finish the iteration
    //        CHECK_EQ(s.getIterativeTaskHandler().getMinHeap().size(), 1);
    //        // The Q size should be set to 2. Since the task is iterative, it needs to be inserted twice in the Q.
    //        CHECK_EQ(s.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::LOWER].queue.size(), 2);
    //        // The Q size should be set to 2. The timeout is 8 seconds, and a 5-second delay is required between inserting tasks.
    //        CHECK_EQ(s.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::MIDDLE].queue.size(), 2);

    //        std::this_thread::sleep_for(std::chrono::milliseconds(5));

    //        // All the task finish iteration
    //        CHECK_EQ(s.getIterativeTaskHandler().getMinHeap().size(), 0);
    //        //CHECK_EQ(higherTask->getStatus(), TaskStatus::COMPLETED);
    //        cout << "higher task: " << higherTask->getStatus() << endl;
    //        cout << "iterarive task1: " << higherTask->getStatus() << endl;
    //        cout << "iterative task2: " << higherTask->getStatus() << endl;
    //        //CHECK_EQ(iterativeTask2->getStatus(), TaskStatus::COMPLETED);
    //        //CHECK_EQ(iterativeTask1->getStatus(), TaskStatus::RUNNING);
    //        std::this_thread::sleep_for(std::chrono::milliseconds(15));

        }
        
        SUBCASE("1.3.3 Mix of Base Task Type and Deadline Tasks Simultaneously") {
            shared_ptr<Task> higherTask(new Task(Scheduler::taskIds++, PrioritiesLevel::HIGHER, 10));
            shared_ptr<Task> middleTask(new Task(Scheduler::taskIds++, PrioritiesLevel::MIDDLE, 1));
            shared_ptr<Task> lowerTask(new Task(Scheduler::taskIds++, PrioritiesLevel::LOWER, 1));
            shared_ptr<DeadLineTask> deadlineTask(new DeadLineTask(Task(Scheduler::taskIds++, PrioritiesLevel::LOWER, 5), time(nullptr) + 10));

            s.insertTask(higherTask);
            s.insertTask(middleTask);
            s.insertTask(lowerTask);
            s.insertTask(deadlineTask);

            std::this_thread::sleep_for(std::chrono::milliseconds(3));

            CHECK_EQ(s.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::LOWER].queue.size(), 2);

            CHECK(higherTask->getStatus() == TaskStatus::RUNNING);

            std::this_thread::sleep_for(std::chrono::milliseconds(70));

            CHECK_EQ(deadlineTask->getPriority(), PrioritiesLevel::CRITICAL);
            CHECK_EQ(deadlineTask->getStatus(), TaskStatus::RUNNING);
            CHECK_EQ(higherTask->getStatus(), TaskStatus::SUSPENDED);

            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            CHECK(deadlineTask->getStatus() == TaskStatus::COMPLETED);
            CHECK(higherTask->getStatus() == TaskStatus::COMPLETED);
            CHECK(middleTask->getStatus() == TaskStatus::COMPLETED);
            CHECK(lowerTask->getStatus() == TaskStatus::COMPLETED);
        }

    //    SUBCASE("1.3.4 Mix of Base Task Type and Ordered Tasks Simultaneously") {
    //        shared_ptr<Task> criticalTask(new Task(Scheduler::taskIds++, PrioritiesLevel::CRITICAL, 10));
    //        shared_ptr<Task> higherTask(new Task(Scheduler::taskIds++, PrioritiesLevel::HIGHER, 10));
    //        shared_ptr<Task> lowerTask1(new Task(Scheduler::taskIds++, PrioritiesLevel::LOWER, 10));
    //        shared_ptr<Task> lowerTask2(new Task(Scheduler::taskIds++, PrioritiesLevel::LOWER, 10));
    //        shared_ptr<Task> orderedTask1(new Task(Scheduler::taskIds++, PrioritiesLevel::LOWER, 1, true));
    //        shared_ptr<Task> orderedTask2(new Task(Scheduler::taskIds++, PrioritiesLevel::HIGHER, 1, true));
    //        shared_ptr<Task> orderedTask3(new Task(Scheduler::taskIds++, PrioritiesLevel::CRITICAL, 1, true));
    //        
    //        s.insertTask(lowerTask1); 
    //        s.insertTask(lowerTask2); 
    //        s.insertTask(orderedTask1);
    //        s.insertTask(higherTask);
    //        s.insertTask(orderedTask2);
    //        s.insertTask(criticalTask);
    //        s.insertTask(orderedTask3);


    //    }
    }

    Init_Scheduler_Thread.detach();
}


