#pragma once
#include <queue>
#include "Task.h"
class RealTimeScheduler
{
private:
    queue<Task*> realTime;
public:
    queue<Task*> getRealTimeQueue() const {
        return realTime;
    }
    void addTask(Task* task) {
        realTime.push(task);
    }
};