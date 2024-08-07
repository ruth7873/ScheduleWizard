#pragma once
#include <queue>
#include "Task.h"
#include "Consts.h"
#include <unordered_map>

typedef struct Q {
    std::queue<Task*> WRRQueue;
    int weight;
} Queue;

class WeightRoundRobinScheduler
{
private:
    std::unordered_map<std::string, Queue> WRRQueues;

public:
    std::unordered_map<std::string, Queue> getWrrQueues() {
        return WRRQueues;
    }

    WeightRoundRobinScheduler();
    void addTask(Task* task);
};
