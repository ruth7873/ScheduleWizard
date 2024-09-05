#include "DeadlineTaskManager.h"
#include "Scheduler.h"
#include <memory> // for std::shared_ptr
#include <iostream>
#include <ctime>

// Definition of the static member
std::priority_queue<std::shared_ptr<DeadLineTask>, std::vector<std::shared_ptr<DeadLineTask>>, std::greater<std::shared_ptr<DeadLineTask>>> DeadlineTaskManager::minHeap;

void DeadlineTaskManager::addTask(const std::shared_ptr<DeadLineTask>& task) {
    minHeap.push(task);
}

void DeadlineTaskManager::deadlineMechanism() {
    if (!minHeap.empty()) {
        std::shared_ptr<DeadLineTask> earliestTask = minHeap.top();
        time_t currentTime = time(nullptr);

        // Check if the current time is close to the task's deadline
        if (currentTime >= earliestTask->getDeadline() - earliestTask->getRunningTime() &&
            earliestTask->getPriority() != PrioritiesLevel::CRITICAL) {

            cout << " ----- DEADLINE COMMIMG ----- " << endl;
            std::cout << "The Task id: " << earliestTask->getId() << " became critical" << std::endl;

            // Change the status of the task
            earliestTask->setPriority(PrioritiesLevel::CRITICAL);

            // Insert the task into the scheduler
            std::shared_ptr<Task> taskPtr = std::make_shared<DeadLineTask>(*earliestTask);
            Scheduler::insertTask(taskPtr);

            // Remove the task from the heap
            minHeap.pop();
        }
        else {
            if (earliestTask->getPriority() != PrioritiesLevel::CRITICAL) {
                minHeap.pop();

            }
        }
    }
}
