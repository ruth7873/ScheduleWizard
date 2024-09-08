//#include "doctest.h"
//#include <fstream>
//#include <memory>
//#include <thread>
//#include <chrono>
//#include "../Main/ReadFromJSON.h"
//#include "../Main/Scheduler.h"
//
//// Mock JSON Data
//const std::string testJsonFilePath = "test_tasks.json";
//
//// Helper function to create temporary JSON files
//void createTestJSONFile(const std::string& content) {
//	std::ofstream file(testJsonFilePath);
//	file << content;
//	file.close();
//}
//
//// Helper function to remove the temporary file after tests
//void removeTestJSONFile() {
//	std::remove(testJsonFilePath.c_str());
//}
//
//TEST_CASE("Test ReadFromJSON::createTasksFromJSON") {
//	Scheduler scheduler;  // Assuming Scheduler is responsible for managing tasks
//
//	SUBCASE("Successful Reading of Tasks from JSON File") {
//		// Mock JSON data with tasks
//		std::string jsonData = R"(
//	{
//	 "tasks": [
//        {
//            "priority": "Critical",
//            "runningTime": 2
//        },
//        {
//            "priority": "Lower",
//            "runningTime": 2
//        }
//	]
//	})";
//
//		// Create temporary JSON file
//		createTestJSONFile(jsonData);
//
//		// Read tasks from JSON
//		ReadFromJSON reader;
//		reader.createTasksFromJSON(testJsonFilePath);
//
//		// Check if tasks were added to the scheduler correctly
//		CHECK_EQ(Scheduler::totalRunningTask, 2);  // Assuming Scheduler has getTaskCount()
//
//		// Validate the first task
//	//    auto task1 = scheduler.getTask(0);  // Assuming Scheduler has getTask()
//	//    CHECK_EQ(task1->getPriority(), 1);
//	//    CHECK_EQ(task1->getRunningTime(), 2);
//	//    CHECK_EQ(task1->getStatus(), TaskStatus::READY);
//
//	//    // Validate the second task
//	//    auto task2 = scheduler.getTask(1);
//	//    CHECK_EQ(task2->getPriority(), 2);
//	//    CHECK_EQ(task2->getRunningTime(), 3);
//	//    CHECK_EQ(task2->getStatus(), TaskStatus::WAITING);
//
//	//    // Cleanup: remove the temporary JSON file
//	//    removeTestJSONFile();
//	}
//
//	//SUBCASE("Handling of Empty Task JSON Data") {
//	//    // Empty JSON data with no tasks
//	//    std::string jsonData = R"(
//	//    {
//	//        "tasks": []
//	//    })";
//
//	//    // Create temporary JSON file
//	//    createTestJSONFile(jsonData);
//
//	//    // Read tasks from JSON
//	//    ReadFromJSON reader;
//	//    reader.createTasksFromJSON(testJsonFilePath);
//
//	//    // No tasks should have been added
//	//    CHECK_EQ(scheduler.getTaskCount(), 0);
//
//	//    // Cleanup: remove the temporary JSON file
//	//    removeTestJSONFile();
//	//}
//
//	//SUBCASE("Handling of Invalid or Non-Existing JSON File") {
//	//    // Try reading from a non-existing file
//	//    ReadFromJSON reader;
//	//    CHECK_NOTHROW(reader.createTasksFromJSON("non_existing_file.json"));  // Ensure no crash
//
//	//    // No tasks should have been added
//	//    CHECK_EQ(scheduler.getTaskCount(), 0);
//	//}
//}
//
////TEST_CASE("Test ReadFromJSON::createTasksFromJSONWithDelay") {
////    Scheduler scheduler;
////
////    SUBCASE("Successful Reading of Tasks with Delay") {
////        // Mock JSON data with tasks
////        std::string jsonData = R"(
////        {
////            "tasks": [
////                { "priority": 1, "runningTime": 2, "status": "ready" },
////                { "priority": 2, "runningTime": 3, "status": "waiting" }
////            ]
////        })";
////
////        // Create temporary JSON file
////        createTestJSONFile(jsonData);
////
////        // Read tasks from JSON with a delay
////        ReadFromJSON reader;
////        std::thread readerThread([&reader]() {
////            reader.createTasksFromJSONWithDelay(testJsonFilePath, 1, 1, "Reading tasks...");
////            });
////
////        // Allow time for the first task to be read
////        std::this_thread::sleep_for(std::chrono::seconds(2));
////
////        // After 2 seconds, we expect the first task to be inserted
////        CHECK_EQ(scheduler.getTaskCount(), 1);
////
////        // Allow additional time for the second task to be read
////        std::this_thread::sleep_for(std::chrono::seconds(2));
////        CHECK_EQ(scheduler.getTaskCount(), 2);
////
////        readerThread.join();  // Wait for the reader thread to complete
////
////        // Cleanup: remove the temporary JSON file
////        removeTestJSONFile();
////    }
////}
