#pragma once
#include <memory>
#include <queue>
#include <vector>
#include "Scheduler.h"
#include <iostream>
#include <ctime>

class Task;
class DeadLineTask;

class DeadlineTaskManager {
private:
    // Min-heap to store shared pointers of DeadLineTask objects
    static priority_queue<std::shared_ptr<DeadLineTask>, std::vector<std::shared_ptr<DeadLineTask>>, std::greater<std::shared_ptr<DeadLineTask>>> minHeap;

public:
    // Add a new DeadlineTask (as shared pointer) to the heap
    void addTask(const std::shared_ptr<DeadLineTask>& task);

    std::shared_ptr<DeadLineTask> getUpcomingTask();

    // Mechanism to handle deadline tasks
    static void deadlineMechanism();
};
