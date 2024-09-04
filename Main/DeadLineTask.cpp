#include "DeadLineTask.h"

DeadLineTask::DeadLineTask(Task basicTask, int deadLineTime)
	: deadLineTime(deadLineTime), Task(basicTask)
{}
