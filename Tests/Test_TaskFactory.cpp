#include "doctest.h"
#include "../Main/TaskFactory.h"
#include <nlohmann/json.hpp>
#include "../Main/Scheduler.h"

class TaskFactoryFixture {
public:
	TaskFactoryFixture() {
		// Reset the taskIds before each test
		Scheduler::taskIds = 0;
	}
};

TEST_CASE_FIXTURE(TaskFactoryFixture, "Create Basic Task") {
	nlohmann::json taskData = {
		{"type", TaskType::BASIC},
		{"priority", PrioritiesLevel::MIDDLE},
		{"runningTime", 100}
	};

	auto task = TaskFactory::createTask(taskData);

	CHECK(task != nullptr);
	CHECK(task->getPriority() == PrioritiesLevel::MIDDLE);
	CHECK(task->getRunningTime() == 100);
	CHECK_FALSE(task->getIsOrdered());
}

TEST_CASE_FIXTURE(TaskFactoryFixture, "Create Ordered Task") {
	nlohmann::json taskData = {
		{"type", TaskType::ORDERED},
		{"priority", PrioritiesLevel::HIGHER},
		{"runningTime", 200}
	};

	auto task = TaskFactory::createTask(taskData);

	CHECK(task != nullptr);
	CHECK(task->getPriority() == PrioritiesLevel::HIGHER);
	CHECK(task->getRunningTime() == 200);
	CHECK(task->getIsOrdered());
}

TEST_CASE_FIXTURE(TaskFactoryFixture, "Create Deadline Task") {
	nlohmann::json taskData = {
		{"type", TaskType::DEAD_LINE},
		{"priority", PrioritiesLevel::CRITICAL},
		{"runningTime", 150},
		{"deadLine", 1000}
	};

	auto task = TaskFactory::createTask(taskData);

	CHECK(task != nullptr);
	auto deadlineTask = dynamic_pointer_cast<DeadLineTask>(task);
	CHECK(deadlineTask != nullptr);
	CHECK(deadlineTask->getPriority() == PrioritiesLevel::CRITICAL);
	CHECK(deadlineTask->getRunningTime() == 150);
	CHECK(deadlineTask->getDeadline() == 1000);
}

TEST_CASE_FIXTURE(TaskFactoryFixture, "Create Iterative Task") {
	nlohmann::json taskData = {
		{"type", TaskType::ITERATIVE},
		{"priority", PrioritiesLevel::LOWER},
		{"runningTime", 50},
		{"iterationsRemaining", 5},
		{"executionInterval", 100}
	};

	auto task = TaskFactory::createTask(taskData);

	CHECK(task != nullptr);
	auto iterativeTask = dynamic_pointer_cast<IterativeTask>(task);
	CHECK(iterativeTask != nullptr);
	CHECK(iterativeTask->getPriority() == PrioritiesLevel::LOWER);
	CHECK(iterativeTask->getRunningTime() == 50);
	CHECK(iterativeTask->getIterationsRemaining() == 5);
	CHECK(iterativeTask->getExecutionInterval() == 100);
}

TEST_CASE_FIXTURE(TaskFactoryFixture, "Create Invalid Task Type") {
	nlohmann::json taskData = {
		{"type", "INVALID_TYPE"},
		{"priority", PrioritiesLevel::MIDDLE},
		{"runningTime", 100}
	};

	auto task = TaskFactory::createTask(taskData);

	CHECK(task == nullptr);
}

TEST_CASE_FIXTURE(TaskFactoryFixture, "Create basic Task With Missing Fields") {
	nlohmann::json taskData = {
		{"type", TaskType::BASIC},
		{"priority", PrioritiesLevel::MIDDLE}
		// Missing "runningTime" field
	};

	auto task = TaskFactory::createTask(taskData);

	CHECK(task == nullptr);
}

TEST_CASE_FIXTURE(TaskFactoryFixture, "Create besic Task With Missing priority") {
	nlohmann::json taskData = {
		{"type", TaskType::BASIC},
		// Missing "runningTime" field
	};

	auto task = TaskFactory::createTask(taskData);

	CHECK(task == nullptr);
}

TEST_CASE_FIXTURE(TaskFactoryFixture, "Create deadLine Task With Missing Fields") {
	nlohmann::json taskData = {
		{"type", TaskType::DEAD_LINE},
		{"priority", PrioritiesLevel::MIDDLE},
		{ "runningTime", 100 }
	};

	auto task = TaskFactory::createTask(taskData);

	CHECK(task == nullptr);
}
TEST_CASE_FIXTURE(TaskFactoryFixture, "Create deadLine Task With Missing deadLine") {
	nlohmann::json taskData = {
		{"type", TaskType::DEAD_LINE},
		{"priority", PrioritiesLevel::MIDDLE}

		// Missing "runningTime" field
	};

	auto task = TaskFactory::createTask(taskData);

	CHECK(task == nullptr);
}
TEST_CASE_FIXTURE(TaskFactoryFixture, "Create deadLine Task With Missing priority") {
	nlohmann::json taskData = {
		{"type", TaskType::DEAD_LINE},
		// Missing "runningTime" field
	};

	auto task = TaskFactory::createTask(taskData);

	CHECK(task == nullptr);
}

TEST_CASE_FIXTURE(TaskFactoryFixture, "Create iterative Task With Missing Fields") {
	nlohmann::json taskData = {
		{"type", TaskType::ITERATIVE},
		{"priority", PrioritiesLevel::MIDDLE}
		// Missing "runningTime" field
	};

	auto task = TaskFactory::createTask(taskData);

	CHECK(task == nullptr);
}

TEST_CASE_FIXTURE(TaskFactoryFixture, "Create iterative Task With Missing fields") {
	nlohmann::json taskData = {
		{"type", TaskType::ITERATIVE},
		{"priority", PrioritiesLevel::MIDDLE},
		{ "runningTime", 100 }
	};

	auto task = TaskFactory::createTask(taskData);

	CHECK(task == nullptr);
}

TEST_CASE_FIXTURE(TaskFactoryFixture, "Create iterative Task With Missing fields") {
	nlohmann::json taskData = {
		{"type", TaskType::ITERATIVE},
		{"priority", PrioritiesLevel::MIDDLE},
		{ "runningTime", 100 },
		{"iterationsRemaining",3}
	};

	auto task = TaskFactory::createTask(taskData);

	CHECK(task == nullptr);
}

TEST_CASE_FIXTURE(TaskFactoryFixture, "Create iterative Task With Missing priority") {
	nlohmann::json taskData = {
		{"type", TaskType::ITERATIVE},
		// Missing "runningTime" field
	};

	auto task = TaskFactory::createTask(taskData);

	CHECK(task == nullptr);
}

TEST_CASE_FIXTURE(TaskFactoryFixture, "Create basic Task With Invalid Priority") {
	nlohmann::json taskData = {
		{"type", TaskType::BASIC},
		{"priority", "INVALID_PRIORITY"},
		{"runningTime", 100}
	};

	auto task = TaskFactory::createTask(taskData);

	CHECK(task == nullptr);
}
TEST_CASE_FIXTURE(TaskFactoryFixture, "Create deadLine Task With Invalid Priority") {
	nlohmann::json taskData = {
		{"type", TaskType::DEAD_LINE},
		{"priority", "INVALID_PRIORITY"},
		{"runningTime", 100},
		{"deadLine",4}
	};

	auto task = TaskFactory::createTask(taskData);

	CHECK(task == nullptr);
}
TEST_CASE_FIXTURE(TaskFactoryFixture, "Create Iterative Task With Invalid Priority") {
	nlohmann::json taskData = {
		{"type", TaskType::ITERATIVE},
		{"priority", "INVALID_PRIORITY"},
		{"runningTime", 100},
		{"iterationsRemaining", 3},
		{"executionInterval", 2}
	};

	auto task = TaskFactory::createTask(taskData);

	CHECK(task == nullptr);
}

TEST_CASE_FIXTURE(TaskFactoryFixture, "a") {
	nlohmann::json taskData = {"dfdf"
		
	};

	auto task = TaskFactory::createTask(taskData);

	CHECK(task == nullptr);
}

TEST_CASE("Create Task - Handle Exceptions") {
	// Creating json data that is deliberately malformed
	nlohmann::json malformedJson = {
		{"type", TaskType::BASIC},  // Correct type
		{"priority", "Middle"},
		// Missing "runningTime" which should throw an exception
	};

	// Capture the output of exceptions thrown
	std::ostringstream outputBuffer;
	std::streambuf* oldCerrBuffer = std::cerr.rdbuf(outputBuffer.rdbuf()); // Redirect std::cerr to capture error messages

	auto task = TaskFactory::createTask(malformedJson);

	// Check if the task creation failed and returned nullptr
	CHECK(task == nullptr);

	// Restore the original cerr buffer
	std::cerr.rdbuf(oldCerrBuffer);

	// Now you can verify that the exception message was logged
	std::string errorMessage = outputBuffer.str();
	CHECK(errorMessage.find("Invalid Arguments. Please enter one of the specified options.") != std::string::npos);
}

TEST_CASE("Create Task - Invalid JSON Type") {
	nlohmann::json invalidTypeJson = {
		{"type", "INVALID_TYPE"},  // Invalid task type
		{"priority", PrioritiesLevel::MIDDLE},
		{"runningTime", 100}
	};

	// Capture the output of exceptions thrown
	std::ostringstream outputBuffer;
	std::streambuf* oldCerrBuffer = std::cerr.rdbuf(outputBuffer.rdbuf()); // Redirect std::cerr to capture error messages

	auto task = TaskFactory::createTask(invalidTypeJson);

	// Check if the task creation failed and returned nullptr
	CHECK(task == nullptr);

	// Restore the original cerr buffer
	std::cerr.rdbuf(oldCerrBuffer);

	// Verify that the expected error message was logged
	std::string errorMessage = outputBuffer.str();
	CHECK(errorMessage.find("An exception occurred:") != std::string::npos);
}



// Mock the input functions to avoid interactive input during tests
#include <iostream>
#include <sstream>

using namespace std;

TEST_SUITE("TaskFactory Tests") {

	TEST_CASE("Test basicInput function") {
		// Mock input stream
		stringstream mockInput;
		mockInput.str("Critical\n100\n"); // Mock user input: priority = Critical, runningTime = 100
		std::streambuf* oldCinStream = std::cin.rdbuf(mockInput.rdbuf()); // Redirect std::cin

		TaskFactory factory;
		string type = TaskType::BASIC;
		Task task = factory.createTask(type);

		// Verify task properties
		CHECK(task.getPriority() == PrioritiesLevel::CRITICAL);
		CHECK(task.getRunningTime() == 100);

		// Restore std::cin
		std::cin.rdbuf(oldCinStream);
	}

	TEST_CASE("Test basicTaskInput function") {
		// Mock input stream
		stringstream mockInput;
		mockInput.str("Higher\n200\n"); // Mock user input: priority = Higher, runningTime = 200
		std::streambuf* oldCinStream = std::cin.rdbuf(mockInput.rdbuf()); // Redirect std::cin

		TaskFactory factory;
		string type = TaskType::BASIC;
		auto taskPtr = factory.createTask(type);

		// Verify task properties
		CHECK(taskPtr->getPriority() == PrioritiesLevel::HIGHER);
		CHECK(taskPtr->getRunningTime() == 200);

		// Restore std::cin
		std::cin.rdbuf(oldCinStream);
	}

	TEST_CASE("Test deadLineTaskInput function") {
		// Mock input stream
		stringstream mockInput;
		mockInput.str("Middle\n300\n400\n"); // Mock user input: priority = Middle, runningTime = 300, deadline = 400
		std::streambuf* oldCinStream = std::cin.rdbuf(mockInput.rdbuf()); // Redirect std::cin

		TaskFactory factory;
		string type = TaskType::DEAD_LINE;
		auto taskPtr = factory.createTask(type);

		// Verify task properties
		CHECK(taskPtr->getPriority() == PrioritiesLevel::MIDDLE);
		CHECK(taskPtr->getRunningTime() == 300);

		time_t now = time(nullptr);
		//CHECK(taskPtr->getDeadline() == now + 400);

		// Restore std::cin
		std::cin.rdbuf(oldCinStream);
	}

	TEST_CASE("Test iterativeTaskInput function") {
		// Mock input stream
		stringstream mockInput;
		mockInput.str("Lower\n500\n5\n10\n"); // Mock user input: priority = Lower, runningTime = 500, iterationsRemaining = 5, executionInterval = 10
		std::streambuf* oldCinStream = std::cin.rdbuf(mockInput.rdbuf()); // Redirect std::cin

		TaskFactory factory;
		string type = TaskType::ITERATIVE;
		auto taskPtr = factory.createTask(type);

		// Verify task properties
		CHECK(taskPtr->getPriority() == PrioritiesLevel::LOWER);
		CHECK(taskPtr->getRunningTime() == 500);
		//CHECK(taskPtr->getIterationsRemaining() == 5);
		//CHECK(taskPtr->getExecutionInterval() == 10);

		// Restore std::cin
		std::cin.rdbuf(oldCinStream);
	}

	TEST_CASE("Test createTask function") {
		TaskFactory factory;

		// Mock input stream
		stringstream mockInput;
		mockInput.str("Higher\n200\n"); // Mock user input: priority = Higher, runningTime = 200
		std::streambuf* oldCinStream = std::cin.rdbuf(mockInput.rdbuf()); // Redirect std::cin

		auto taskPtr = factory.createTask(TaskType::ORDERED);
		CHECK(taskPtr->getPriority() == PrioritiesLevel::HIGHER);
		CHECK(taskPtr->getRunningTime() == 200);

		// Restore std::cin
		std::cin.rdbuf(oldCinStream);
	}
}
TEST_CASE("Test createTask function") {
	TaskFactory factory;

	// Mock input stream
	stringstream mockInput;
	mockInput.str("Higher\n200\n"); // Mock user input: priority = Higher, runningTime = 200
	std::streambuf* oldCinStream = std::cin.rdbuf(mockInput.rdbuf()); // Redirect std::cin

	string type = "other type";
	auto taskPtr = factory.createTask(type);
	CHECK(taskPtr == nullptr);

	// Restore std::cin
	std::cin.rdbuf(oldCinStream);
}

TEST_CASE("Test basicInput with invalid priority") {
	// Mock input stream with invalid priority then valid priority
	stringstream mockInput;
	mockInput.str("Invalid\nInvalid\nCritical\n150\n"); // Mock user input: two invalid priorities followed by valid ones
	std::streambuf* oldCinStream = std::cin.rdbuf(mockInput.rdbuf()); // Redirect std::cin

	TaskFactory factory;
	string type = TaskType::BASIC;
	Task task = factory.createTask(type);

	// Verify task properties
	CHECK(task.getPriority() == PrioritiesLevel::CRITICAL);
	CHECK(task.getRunningTime() == 150);

	// Restore std::cin
	std::cin.rdbuf(oldCinStream);
}
