#pragma once
#include "DeadLineTask.h"
#include <memory>
#include <queue>
#include <vector>

class DeadlineTaskManager {
private:
    // Min-heap to store shared pointers of DeadLineTask objects
    static priority_queue<std::shared_ptr<DeadLineTask>, std::vector<std::shared_ptr<DeadLineTask>>, std::greater<std::shared_ptr<DeadLineTask>>> minHeap;

public:
    // Add a new DeadlineTask (as shared pointer) to the heap
    void addTask(const std::shared_ptr<DeadLineTask>& task);

    // Mechanism to handle deadline tasks
    static void deadlineMechanism();
};
