#include "DeadLineTask.h"
#include <memory> // for std::shared_ptr


// Definition of the static member
//DeadlineTaskManager DeadLineTask::manager;

DeadLineTask::DeadLineTask(Task basicTask, int deadLineTime)
    : Task(basicTask.getId(), basicTask.getPriority(), basicTask.getRunningTime(), basicTask.getStatus()),
    deadline(deadLineTime) {
    // Automatically add the task to the manager's min-heap when it's created
    //manager.addTask(std::make_shared<DeadLineTask>(*this));
}

time_t DeadLineTask::getDeadline() const {
    return deadline;
}

bool DeadLineTask::operator>(const DeadLineTask& other) const {
    return deadline > other.deadline;
}
