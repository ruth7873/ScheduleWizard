#pragma once
#include "Task.h"
class DeadLineTask: public Task
{
	int deadLineTime;
public:
	DeadLineTask(Task basicTask, int deadLineTime);
};

