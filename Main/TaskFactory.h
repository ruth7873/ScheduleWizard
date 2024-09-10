#pragma once
#include "DeadLineTask.h"
#include "Utility.h"
#include "IterativeTask.h"
#include <memory>
#include <nlohmann/json.hpp>
class TaskFactory
{
private:
	static Task basicInput();
	static shared_ptr<Task> basicTaskInput();
	static shared_ptr<DeadLineTask> deadLineTaskInput();
	static shared_ptr<IterativeTask> iterativeTaskInput();

public:
	static shared_ptr<Task> createTask(string taskType);
	static shared_ptr<Task> createTask( const nlohmann::json& taskData);
};

