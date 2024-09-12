#include "DeadlineTaskManager.h"


// Definition of the static member
std::priority_queue<std::shared_ptr<DeadLineTask>, std::vector<std::shared_ptr<DeadLineTask>>, DeadlineTaskManager::CompareDeadline> DeadlineTaskManager::minHeap;

void DeadlineTaskManager::addTask(const std::shared_ptr<DeadLineTask>& task) {
    if (task->getPriority() != PrioritiesLevel::CRITICAL)
        minHeap.push(task);
}

std::shared_ptr<DeadLineTask> DeadlineTaskManager::getUpcomingTask() {
    if (minHeap.empty())
        throw "Heap is empty";
    return minHeap.top();
    
}

void DeadlineTaskManager::deadlineMechanism() {
    if (!minHeap.empty()) {
        std::shared_ptr<DeadLineTask> earliestTask = minHeap.top();
        time_t currentTime = time(nullptr);
        // Check if the current time is close to the task's deadline
        if (currentTime >= earliestTask->getDeadline() - earliestTask->getRunningTime() &&
            earliestTask->getPriority() != PrioritiesLevel::CRITICAL &&
            earliestTask->getStatus() != TaskStatus::COMPLETED &&
            earliestTask->getStatus() != TaskStatus::TERMINATED) {

            // Change the status of the task
            earliestTask->setPriority(PrioritiesLevel::CRITICAL);

            // Insert the task into the scheduler
            Scheduler::insertTask(dynamic_pointer_cast<Task>(earliestTask));
            // Remove the task from the heap
            {
                std::unique_lock<std::mutex> lock(Scheduler::rtLock);  // Lock the rtLock
                if (!minHeap.empty())
                    minHeap.pop();
            }
        }
        else {
            if (earliestTask->getPriority() == PrioritiesLevel::CRITICAL ||
                earliestTask->getStatus() == TaskStatus::COMPLETED)

                minHeap.pop();
        }
    }
}
