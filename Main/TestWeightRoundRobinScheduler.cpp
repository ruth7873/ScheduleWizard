#include "doctest.h"
#include "Scheduler.h"
#include "WeightRoundRobinScheduler.h"
#include "Consts.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include <atomic>


TEST_CASE("Test WeightRoundRobinScheduler with Log File Verification") {
    Scheduler scheduler;
    WeightRoundRobinScheduler WRRscheduler;

    // Redirect spdlog to a test log file
    auto logger = spdlog::basic_logger_mt("test_logger", "test_log.txt");
    spdlog::set_default_logger(logger);

    SUBCASE("Run tasks and verify execution order in the log file") {
        auto higherTask1 = std::make_shared<Task>(1, PrioritiesLevel::HIGHER, 1);
        auto higherTask2 = std::make_shared<Task>(2, PrioritiesLevel::HIGHER, 5);
        auto middleTask1 = std::make_shared<Task>(3, PrioritiesLevel::MIDDLE, 2);
        auto lowerTask1 = std::make_shared<Task>(4, PrioritiesLevel::LOWER, 3);

        WRRscheduler.addTask(higherTask1);
        WRRscheduler.addTask(higherTask2);
        WRRscheduler.addTask(middleTask1);
        WRRscheduler.addTask(lowerTask1);

        // Run the WeightRoundRobinScheduler function to process tasks
        std::thread schedulerThread(&WeightRoundRobinScheduler::weightRoundRobinFunction, &WRRscheduler);
             
        // Give some time for the tasks to be processed
        std::this_thread::sleep_for(std::chrono::seconds(15));

        // Stop the scheduler thread after processing
        schedulerThread.detach();

        // Open and analyze the log file
        std::ifstream logFile("test_log.txt");
        REQUIRE(logFile.is_open());

        std::string line;
        int higherTaskCount = 0;
        int middleTaskCount = 0;
        int lowerTaskCount = 0;

        while (std::getline(logFile, line)) {
            if (line.find("Task with ID: 1 completed.") != std::string::npos ||
                line.find("Task with ID: 2 completed.") != std::string::npos) {
                higherTaskCount++;
            }
            else if (line.find("Task with ID: 3 completed.") != std::string::npos) {
                middleTaskCount++;
            }
            else if (line.find("Task with ID: 4 completed.") != std::string::npos) {
                lowerTaskCount++;
            }
        }

        logFile.close();

        // Higher priority tasks should have a higher count executed
        CHECK_GT(higherTaskCount, middleTaskCount);
        CHECK_GT(middleTaskCount, lowerTaskCount);

        // Clean up the log file
        std::remove("test_log.txt");
    }
    SUBCASE("Insert tasks into the correct queue based on priority") {

        shared_ptr<Task> higherTask(new Task(1, PrioritiesLevel::HIGHER, 5));
        shared_ptr<Task> middleTask(new Task(2, PrioritiesLevel::MIDDLE, 10));
        shared_ptr<Task> lowerTask(new Task(3, PrioritiesLevel::LOWER, 15));

        scheduler.insertTask(higherTask);
        scheduler.insertTask(middleTask);
        scheduler.insertTask(lowerTask);

        CHECK_EQ(scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::HIGHER].queue.size(), 1);
        CHECK_EQ(scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::MIDDLE].queue.size(), 1);
        CHECK_EQ(scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::LOWER].queue.size(), 1);
    }
}
