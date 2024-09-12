#pragma once
#include <memory>
#include <queue>
#include <vector>
#include "Scheduler.h"
#include <iostream>
#include <ctime>

class DeadlineTaskManager {
private:
    // Min-heap to store shared pointers of DeadLineTask objects
    struct CompareDeadline {
        bool operator()(const std::shared_ptr<DeadLineTask>& lhs, const std::shared_ptr<DeadLineTask>& rhs) const {

            return lhs->DeadLineTask::getDeadline() > rhs->DeadLineTask::getDeadline(); // Min-heap based on deadline
            
        }
    };

    static std::priority_queue<std::shared_ptr<DeadLineTask>, std::vector<std::shared_ptr<DeadLineTask>>, CompareDeadline> minHeap;

public:
    // Add a new DeadlineTask (as shared pointer) to the heap
    void addTask(const std::shared_ptr<DeadLineTask>& task);

    std::shared_ptr<DeadLineTask> getUpcomingTask();

    // Mechanism to handle deadline tasks
    static void deadlineMechanism();
};