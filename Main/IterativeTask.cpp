#include "IterativeTask.h"

IterativeTask::IterativeTask(Task basicTask, int iterationsRemaining, int executionInterval)
    : Task(basicTask.getId(), basicTask.getPriority(), basicTask.getRunningTime(), basicTask.getStatus()),
    iterationsRemaining(iterationsRemaining),
    executionInterval(executionInterval)
{
}
