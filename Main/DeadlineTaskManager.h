#pragma once
#include <memory>
#include <queue>
#include <vector>
#include "Scheduler.h"
#include <iostream>
#include <ctime>
#include "DeadlineTask.h"

class DeadlineTaskManager {
private:
    // Min-heap to store shared pointers of DeadlineTask objects
    struct CompareDeadline {
        bool operator()(const std::shared_ptr<DeadlineTask>& lhs, const std::shared_ptr<DeadlineTask>& rhs) const {

            return lhs->DeadlineTask::getDeadline() > rhs->DeadlineTask::getDeadline(); // Min-heap based on deadline
            
        }
    };

    static std::priority_queue<std::shared_ptr<DeadlineTask>, std::vector<std::shared_ptr<DeadlineTask>>, CompareDeadline> minHeap;

public:
    // Add a new DeadlineTask (as shared pointer) to the heap
    void addTask(const std::shared_ptr<DeadlineTask>& task);

    std::shared_ptr<DeadlineTask> getUpcomingTask();

    // Mechanism to handle deadline tasks
    static void deadlineMechanism();
};