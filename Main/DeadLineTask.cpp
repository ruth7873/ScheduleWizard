#include "DeadLineTask.h"

DeadLineTask::DeadLineTask(Task basicTask, int deadLineTime)
    : Task(basicTask.getId(), basicTask.getPriority(), basicTask.getRunningTime(), basicTask.getStatus()),
	deadLineTime(deadLineTime)
{}
