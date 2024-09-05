#pragma once
#include "Task.h"
class IterativeTask: public Task
{
private:
	int iterationsRemaining;
	int executionInterval;
public:
	IterativeTask(Task basictask, int iterationsRemaining, int executionInterval);
};

