#pragma once
#include <queue>
#include "Task.h"

class RealTimeScheduler
{
private:
    std::queue<Task*> realTimeQueue;

public:
    std::queue<Task*> getRealTimeQueue() const {
        return realTimeQueue;
    }

    void addTask(Task* task) {
        realTimeQueue.push(task);
    }

    void realTimeSchedulerFunction();
};
