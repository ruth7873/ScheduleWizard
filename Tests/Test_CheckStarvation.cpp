//#include "doctest.h"
//#include <memory>
//#include <queue>
//#include <thread>
//#include <chrono>
//#include <atomic>
//#include "../Main/Scheduler.h"
//#include "../Main/Task.h"
//
//class TestScheduler : public Scheduler {
//public:
//    TestScheduler() : Scheduler(nullptr, nullptr) {}
//
//    using Scheduler::checkStarvation;
//    using Scheduler::getStarvationCheckQueue;
//    using Scheduler::setStarvationCheckQueue;
//    using Scheduler::tasksCounter;
//    static const int STARVATION = 10; // Assuming this is the value
//
//    bool starvationDetected{ false };
//    bool stopThread{ false };
//
//    void runCheckStarvation() {
//        while (!stopThread) {
//            try {
//                checkStarvation();
//            }
//            catch (const std::runtime_error&) {
//                starvationDetected = true;
//                break;
//            }
//            std::this_thread::sleep_for(std::chrono::milliseconds(10));
//        }
//    }
//};
//
//// Helper function to create a task
//std::shared_ptr<Task> createTask(int id, string status) {
//    auto task = std::make_shared<Task>(id, PrioritiesLevel::LOWER, 10);
//    task->setStatus(status);
//    return task;
//}
//
//TEST_CASE("Starvation Check Tests") {
//    TestScheduler scheduler;
//
//    SUBCASE("Starvation detected after threshold") {
//        auto task = createTask(1, TaskStatus::CREATION);
//        std::queue<std::shared_ptr<Task>> queue;
//        queue.push(task);
//        scheduler.setStarvationCheckQueue(queue);
//        scheduler.tasksCounter = TestScheduler::STARVATION;
//
//        std::thread t(&TestScheduler::runCheckStarvation, &scheduler);
//
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//
//        scheduler.stopThread = true;
//        t.detach();
//
//        CHECK(scheduler.starvationDetected);
//    }
//    SUBCASE("No starvation when queue is empty") {
//        scheduler.setStarvationCheckQueue(std::queue<std::shared_ptr<Task>>());
//        std::thread t(&TestScheduler::runCheckStarvation, &scheduler);
//
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//
//        scheduler.stopThread = true;
//        t.detach();
//
//        CHECK_FALSE(scheduler.starvationDetected);
//    }
//
//    SUBCASE("No starvation when task just created") {
//
//        auto task = createTask(1, TaskStatus::CREATION);
//        std::queue<std::shared_ptr<Task>> queue;
//        queue.push(task);
//        scheduler.setStarvationCheckQueue(queue);
//        scheduler.tasksCounter = 0;
//
//        std::thread t(&TestScheduler::runCheckStarvation, &scheduler);
//
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//
//        scheduler.stopThread = true;
//        t.detach();
//
//        CHECK_FALSE(scheduler.starvationDetected);
//    }
//   
//    
//    
//    SUBCASE("No starvation when task not in CREATION status") {
//        auto task = createTask(1, TaskStatus::RUNNING);
//        std::queue<std::shared_ptr<Task>> queue;
//        queue.push(task);
//        scheduler.setStarvationCheckQueue(queue);
//        scheduler.tasksCounter = TestScheduler::STARVATION;
//
//        std::thread t(&TestScheduler::runCheckStarvation, &scheduler);
//
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//
//        scheduler.stopThread = true;
//        t.detach();
//
//        CHECK_FALSE(scheduler.starvationDetected);
//    }
//}