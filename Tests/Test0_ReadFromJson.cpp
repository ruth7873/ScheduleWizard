#include "doctest.h"
#include <fstream>
#include <memory>
#include <thread>
#include <chrono>
#include "../Main/ReadFromJSON.h"
#include "../Main/Scheduler.h"
#include "Helper.h"

// Mock JSON Data
const std::string testJsonFilePath = "test_tasks.json";

// Helper function to create temporary JSON files
void createTestJSONFile(const std::string& content) {
	std::ofstream file(testJsonFilePath);
	file << content;
	file.close();
}

// Helper function to remove the temporary file after tests
void removeTestJSONFile() {
	std::remove(testJsonFilePath.c_str());
}

TEST_CASE("Test ReadFromJSON::createTasksFromJSON") {
	Scheduler scheduler(new ReadFromJSON(), new Utility());
	Scheduler::totalRunningTask = 0;
	SUBCASE("Handling of Empty Task JSON Data") {
		clearAll(scheduler);
		// Empty JSON data with no tasks
		std::string jsonData = R"(
	    {
	        "tasks": []
	    })";

		// Create temporary JSON file
		createTestJSONFile(jsonData);

		// Read tasks from JSON
		ReadFromJSON reader;
		reader.createTasksFromJSON(testJsonFilePath);

		// No tasks should have been added
		CHECK_EQ(Scheduler::totalRunningTask, 0);

		// Cleanup: remove the temporary JSON file
		removeTestJSONFile();
	}

	SUBCASE("Handling of Invalid or Non-Existing JSON File") {
		// Try reading from a non-existing file
		ReadFromJSON reader;
		CHECK_NOTHROW(reader.createTasksFromJSON("non_existing_file.json"));  // Ensure no crash

		// No tasks should have been added
		CHECK_EQ(Scheduler::totalRunningTask, 0);
	}

	SUBCASE("Successful Reading of Tasks from JSON File") {
		// Mock JSON data with tasks
		std::string jsonData = R"(
	{
	 "tasks": [
        {
		    "type": "basic",
            "priority": "Critical",
            "runningTime": 2
        },
        {
			"type": "basic",
            "priority": "Lower",
            "runningTime": 3
        }
	]
	})";

		// Create temporary JSON file
		createTestJSONFile(jsonData);

		// Read tasks from JSON
		ReadFromJSON reader;
		reader.createTasksFromJSON(testJsonFilePath);

		// Check if tasks were added to the scheduler correctly
		CHECK_EQ(Scheduler::totalRunningTask, 2);  // Assuming Scheduler has getTaskCount()

		////Validate the first task
		auto task1 = scheduler.getRealTimeScheduler().getRealTimeQueue().front();  // Assuming Scheduler has getTask()
		CHECK_EQ(task1->getPriority(), PrioritiesLevel::CRITICAL);
		CHECK_EQ(task1->getRunningTime(), 2);
		CHECK_EQ(task1->getStatus(), TaskStatus::CREATION);

		// Validate the second task
		auto task2 = scheduler.getWrrQueuesScheduler().getWrrQueues()[PrioritiesLevel::LOWER].queue.front();
		CHECK_EQ(task2->getPriority(), PrioritiesLevel::LOWER);
		CHECK_EQ(task2->getRunningTime(), 3);
		CHECK_EQ(task2->getStatus(), TaskStatus::CREATION);

		// Cleanup: remove the temporary JSON file
		removeTestJSONFile();
		Scheduler::totalRunningTask = 0;

	}

	SUBCASE("Handling of Creating null task ") {
		Scheduler::totalRunningTask = 0;
		// Mock JSON data with tasks
		std::string jsonData = R"(
	{
	 "tasks": [
        {
		    "type": "other",
            "priority": "Critical",
            "runningTime": 2
        } 
	]
	})";

		// Create temporary JSON file
		createTestJSONFile(jsonData);

		// Read tasks from JSON
		ReadFromJSON reader;
		

		// Check if tasks were added to the scheduler correctly
		CHECK_NOTHROW(reader.createTasksFromJSON(testJsonFilePath););

		// Cleanup: remove the temporary JSON file
		removeTestJSONFile();
		Scheduler::totalRunningTask = 0;

	}

	SUBCASE("Successful Reading of Tasks with Delay") {
		Scheduler::totalRunningTask = 0;
		// Mock JSON data with tasks and delays
		std::string jsonData = R"(
        {
            "tasks": [
                { "type": "basic", "priority": "Critical", "runningTime": 2, "delay": 3 },
                { "type": "basic", "priority": "Lower", "runningTime": 3 }
            ]
        })";

		// Create temporary JSON file
		createTestJSONFile(jsonData);

		// Read tasks from JSON with a delay
		ReadFromJSON reader;
		std::thread readerThread([&reader]() {
			reader.createTasksFromJSON(testJsonFilePath);
			});

		readerThread.detach();  // Wait for the reader thread to complete
		
		// Allow time for the first task to be read
		std::this_thread::sleep_for(std::chrono::seconds(1));

		// After 2 seconds, we expect the first task to be inserted
		CHECK_EQ(Scheduler::totalRunningTask, 1);

		
		// Allow additional time for the second task to be read
		std::this_thread::sleep_for(std::chrono::seconds(3));
		CHECK_EQ(Scheduler::totalRunningTask, 2);


		// Cleanup: remove the temporary JSON file
		removeTestJSONFile();
		Scheduler::totalRunningTask = 0;
	}
}


