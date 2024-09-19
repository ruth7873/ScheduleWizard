#pragma once

#include <memory>
#include <queue>
#include "Task.h"
#include "Scheduler.h"

class OrderedTaskHandler
{
    std::queue<std::shared_ptr<Task>> orderedTaskQueue;

public:
    void addOrderedTask(std::shared_ptr<Task>& task);
    void popOrderedTask();
    std::shared_ptr<Task> frontOrderedTask();
};
