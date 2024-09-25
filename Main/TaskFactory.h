#pragma once
#include "DeadlineTask.h"
#include "Utility.h"
#include "IterativeTask.h"
#include <memory>
#include <nlohmann/json.hpp>
#include "Scheduler.h"


class TaskFactory {
private:
	static Task basicInput();
	static shared_ptr<Task> basicTaskInput(bool);
	static shared_ptr<DeadlineTask> deadlineTaskInput();
	static shared_ptr<IterativeTask> iterativeTaskInput();

public:
	static shared_ptr<Task> createTask(const std::string& taskType);
	static shared_ptr<Task> createTask(const nlohmann::json& taskData);

};

