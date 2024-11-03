#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include "Task.h"
#include "Consts.h"
#include "Scheduler.h"
#include "IterativeTask.h"

class Task;
typedef struct Queue {
    std::queue<std::shared_ptr<Task>> queue;
    int weight;
} Queue;

class WeightRoundRobinScheduler {
private:
    std::unordered_map<std::string, Queue> WRRQueues;

public:
    WeightRoundRobinScheduler();
    WeightRoundRobinScheduler(WeightRoundRobinScheduler&& wrr) = delete;
    WeightRoundRobinScheduler(const WeightRoundRobinScheduler& wrr) = delete;
    ~WeightRoundRobinScheduler();

    void addTask(std::shared_ptr<Task> task);
    std::unordered_map<std::string, Queue>& getWrrQueues();
    void weightRoundRobinFunction();
};
