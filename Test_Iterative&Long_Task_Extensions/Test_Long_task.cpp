//#include "doctest.h"
//#include "LongTaskHandler.h"
//#include "Task.h"
//#include "Scheduler.h"
//
//TEST_CASE("Testing LongTaskHandler functionality") {
//    //Clear the static variables before each test
//    LongTaskHandler::setSumOfAllSeconds(0);
//    LongTaskHandler::setNumOfSeconds(0);
//    LongTaskHandler::setAverageLength(0.0);
//
//    SUBCASE("Test average length calculation") {
//        // Setup
//        LongTaskHandler::addSumOfAllSeconds(30);
//        Scheduler::totalRunningTask = 10;
//        LongTaskHandler::calculateAverageLength();
//
//        // Verify average length
//        CHECK(LongTaskHandler::getAverageLength() == doctest::Approx(3.0));
//    }
//
//    SUBCASE("Test haveToSuspendLongTask function") {
//        // Setup
//        auto task = std::make_shared<Task>(1, PrioritiesLevel::HIGHER, 10);
//        LongTaskHandler::addSumOfAllSeconds(30);
//        Scheduler::totalRunningTask = 5;
//        LongTaskHandler::setAverageLength(5.0);
//
//        // Case where task should not be suspended
//        CHECK_FALSE(LongTaskHandler::haveToSuspendLongTask(task));
//
//        // Adjust to make it suspend
//        LongTaskHandler::setNumOfSeconds(6);
//        CHECK(LongTaskHandler::haveToSuspendLongTask(task));
//    }
//
//    SUBCASE("Test stopLongTask function") {
//        // Setup
//        auto task = std::make_shared<Task>(1, PrioritiesLevel::MIDDLE, 10);
//        Scheduler::totalRunningTask = 1;
//        Scheduler::addTaskToItsQueue(task);  // Add to queue first
//
//        // Stop the long task
//        LongTaskHandler::stopLongTask(task);
//
//        // Verify status
//        CHECK(task->getStatus() == TaskStatus::SUSPENDED);
//        // Optionally verify that the task was moved in the queue
//        // This would require checking the actual contents of the scheduler's queue
//    }
//}
