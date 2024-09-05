#include "Deadline.h"
#include <stdexcept> // for std::runtime_error
#include "Scheduler.h" // Assuming Scheduler is defined elsewhere

// Definition of the static member variable
priority_queue<DeadlineTask, vector<DeadlineTask>, greater<DeadlineTask>> DeadlineTask::minHeap;

DeadlineTask::DeadlineTask(int id, string priority, int runtime, time_t deadline)
    : Task(id, priority, runtime), deadline(deadline) {}

time_t DeadlineTask::getDeadline() const {
    return deadline;
}

bool DeadlineTask::operator>(const DeadlineTask& other) const {
    return deadline > other.deadline;
}

void DeadlineTask::addTask(const DeadlineTask& task) {
    minHeap.push(task);
}

DeadlineTask DeadlineTask::popEarliestTask() {
    if (minHeap.empty()) {
        throw runtime_error("No tasks available");
    }
    DeadlineTask earliestTask = minHeap.top();
    minHeap.pop();
    return earliestTask;
}

bool DeadlineTask::isEmpty() const {
    return minHeap.empty();
}

size_t DeadlineTask::getTaskCount() const {
    return minHeap.size();
}

void DeadlineTask::deadlineMechanism() {
    if (!minHeap.empty()) {
        DeadlineTask earliestTask = minHeap.top();
        time_t currentTime = time(nullptr);

        // Check if the current time is close to the task's deadline
        if (currentTime >= earliestTask.getDeadline() - earliestTask.getRunningTime() && earliestTask.getStatus() != TaskStatus::COMPLETED && earliestTask.getStatus() != TaskStatus::RUNNING) {
            cout << " Deadline --- " << endl;
            cout << " The Task id: " << earliestTask.getId() << " became ciritcal "<<endl;
            // Change the status of the task
            earliestTask.setPriority(PrioritiesLevel::CRITICAL);
            // Insert the task into the scheduler
            shared_ptr<Task> taskPtr = make_shared<DeadlineTask>(earliestTask);
            Scheduler::insertTask(taskPtr);
            // Remove the task from the heap
            minHeap.pop();
        }
    }
}