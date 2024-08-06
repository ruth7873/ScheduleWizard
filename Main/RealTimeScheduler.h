
#pragma once
#include <queue>
#include "Task.h"
#include "Scheduler.h"


class RealTimeScheduler
{
private:
    queue<Task*> realTimeQueue;

public:
    queue<Task*> getRealTimeQueue() const {
        return realTimeQueue;
    }

    void addTask(Task* task) {
        realTimeQueue.push(task);
    }
    void realTimeSchedulerFunction();
};
