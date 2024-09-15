#include "doctest.h"
#include "../Main/Task.h"
#include "../Main/Scheduler.h"
#include "../Main/LongTaskHandler.h"
#include "Helper.h"

TEST_CASE("Testing LongTaskHandler functionality") {
    Scheduler scheduler(new ReadFromJSON(), new Utility());
    clearAll(scheduler);
    SUBCASE("Test average length calculation") {
        // Setup
        LongTaskHandler::addSumOfAllSeconds(30);
        Scheduler::totalRunningTask = 10;
        LongTaskHandler::calculateAverageLength();

        // Verify average length
        CHECK(LongTaskHandler::getAverageLength() == doctest::Approx(3.0));
    }

    SUBCASE("Test haveToSuspendLongTask function") {
        // Setup
        auto task1 = std::make_shared<Task>(1, PrioritiesLevel::HIGHER, 10);
        auto task2 = std::make_shared<Task>(1, PrioritiesLevel::CRITICAL, 10);

        //case where task1 should not be suspended because the totalRunningTask is 1.
        Scheduler::totalRunningTask = 1;
        CHECK_FALSE(LongTaskHandler::haveToSuspendLongTask(task1));

        //case where task2 should not be suspended because it's the only one rt task.
        Scheduler::totalRunningTask = 2;
        Scheduler::insertTask(task2);
        CHECK_FALSE(LongTaskHandler::haveToSuspendLongTask(task2));


        LongTaskHandler::addSumOfAllSeconds(30);
        Scheduler::totalRunningTask = 5;
        LongTaskHandler::setAverageLength(5.0);

        // Case where task should not be suspended because of the calculation of the seconds.
        CHECK_FALSE(LongTaskHandler::haveToSuspendLongTask(task1));

        // Adjust to make it suspend
        LongTaskHandler::setNumOfSeconds(6);
        CHECK(LongTaskHandler::haveToSuspendLongTask(task1));
    }

    SUBCASE("Test stopLongTask function") {
        // Setup
        auto task = std::make_shared<Task>(1, PrioritiesLevel::MIDDLE, 10);
        Scheduler::totalRunningTask = 1;
        Scheduler::addTaskToItsQueue(task);  // Add to queue first

        // Stop the long task
        LongTaskHandler::stopLongTask(task);

        // Verify status
        CHECK(task->getStatus() == TaskStatus::SUSPENDED);
        // Optionally verify that the task was moved in the queue
        // This would require checking the actual contents of the scheduler's queue
    }

    // Clear static variables before each test
    LongTaskHandler::setSumOfAllSeconds(0);
    LongTaskHandler::setNumOfSeconds(0);
    LongTaskHandler::setAverageLength(0.0);

    SUBCASE("Test addSumOfAllSeconds and increaseNumOfSeconds") {
        LongTaskHandler::addSumOfAllSeconds(10);
        CHECK(LongTaskHandler::getSumOfAllSeconds() == 10);

        LongTaskHandler::increaseNumOfSeconds();
        CHECK(LongTaskHandler::getNumOfSeconds() == 1);

        LongTaskHandler::increaseNumOfSeconds();
        CHECK(LongTaskHandler::getNumOfSeconds() == 2);
    }

    SUBCASE("Test setters and getters") {
        LongTaskHandler::setSumOfAllSeconds(100);
        CHECK(LongTaskHandler::getSumOfAllSeconds() == 100);

        LongTaskHandler::setNumOfSeconds(5);
        CHECK(LongTaskHandler::getNumOfSeconds() == 5);

        LongTaskHandler::setAverageLength(20.5);
        CHECK(LongTaskHandler::getAverageLength() == doctest::Approx(20.5));
    }
}
