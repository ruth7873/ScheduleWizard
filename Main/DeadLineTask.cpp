#include "DeadlineTask.h"



// Definition of the static member
//DeadlineTaskManager DeadlineTask::manager;

DeadlineTask::DeadlineTask(Task basicTask, int deadLineTime)
    : Task(basicTask.getId(), basicTask.getPriority(), basicTask.getRunningTime(), basicTask.getStatus()),
    deadline(deadLineTime * 100) {
    // Automatically add the task to the manager's min-heap when it's created
    //manager.addTask(std::make_shared<DeadlineTask>(*this));
}

time_t DeadlineTask::getDeadline() const {
    return deadline;
}
bool DeadlineTask::operator>(const DeadlineTask& other) const {
    return deadline < other.deadline;
}

