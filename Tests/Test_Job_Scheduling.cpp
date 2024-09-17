//#include "doctest.h"
//#include "fakeit.hpp"
//#include "../Main/ReadFromJSON.h"
//#include "../Main/Task.h"
//#include "../Main/Scheduler.h"
//#include <iostream>
//#include <sstream>
//
//using namespace std;
//using namespace fakeit;
//
//TEST_CASE("Job Scheduling Test") {
//    // Mock the ReadFromJSON class
//    Mock<ReadFromJSON> mockReadFromJSON;
//
//    // Specify the lambda function's type explicitly
//    auto createTasksLambda = [&](const std::string& filePath) -> void {};
//
//    When(Method(mockReadFromJSON, createTasksFromJSON)).AlwaysDo(createTasksLambda);
//
//    // Replace the actual ReadFromJSON object in the Scheduler with the mock object
//    ReadFromJSON& readFromJSONMock = mockReadFromJSON.get();
//
//    Mock<Utility> mockUtility;
//
//    auto insertTaskFromInputLambda = [&]() -> void {};
//
//    When(Method(mockUtility, insertTaskFromInput)).AlwaysDo(insertTaskFromInputLambda);
//
//    Utility& utilityMock = mockUtility.get();
//
//    // Set the mock object in the Scheduler
//    Scheduler s(&readFromJSONMock, &utilityMock);
//
//    // Call the function that should trigger createTasksFromJSON
//    std::thread Init_Scheduler_Thread([&s]() {
//        SetThreadDescription(GetCurrentThread(), L"Init Scheduler");
//        spdlog::info(Logger::LoggerInfo::START_READ_FROM_JSON_THREAD);
//        s.init();
//        });
//
//    SUBCASE("Real-Time Job Submission") {
//        shared_ptr<Task> criticalTask(new Task(Scheduler::taskIds++, PrioritiesLevel::CRITICAL, 2));
//        shared_ptr<Task> nonCriticalTask(new Task(Scheduler::taskIds++, PrioritiesLevel::HIGHER, 1));
//
//        s.insertTask(criticalTask);
//        s.insertTask(nonCriticalTask);
//
//
//        CHECK((criticalTask->getStatus() == TaskStatus::RUNNING || criticalTask->getStatus() == TaskStatus::CREATION));
//        CHECK(nonCriticalTask->getStatus() == TaskStatus::CREATION);
//
//        std::this_thread::sleep_for(std::chrono::seconds(2));
//
//        CHECK((criticalTask->getStatus() == TaskStatus::RUNNING || criticalTask->getStatus() == TaskStatus::COMPLETED));
//        CHECK((nonCriticalTask->getStatus() == TaskStatus::CREATION || nonCriticalTask->getStatus() == TaskStatus::RUNNING));
//    }
//
//    SUBCASE("Non-Real-Time Job Submission") {
//        // Add test cases for non-real-time job submission
//    }
//
//    SUBCASE("Scheduling Mechanisms") {
//        // Add test cases for scheduling mechanisms
//    }
//
//    Init_Scheduler_Thread.detach();
//}
