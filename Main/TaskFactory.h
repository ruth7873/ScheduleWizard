#pragma once
#include "Task.h"
#include "DeadLineTask.h"
#include "Utility.h"
#include "IterativeTask.h"
#include <memory>
class TaskFactory
{
private:
	static Task basicInput();
	static shared_ptr<Task> basicTaskInput();
	static shared_ptr<DeadLineTask> deadLineTaskInput();
	static shared_ptr<IterativeTask> iterativeTaskInput();
public:
	static shared_ptr<Task> createTask(string type);
};

