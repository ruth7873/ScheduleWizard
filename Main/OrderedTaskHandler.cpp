#include "OrderedTaskHandler.h"

void OrderedTaskHandler::addOrderedTask(const std::shared_ptr<Task>& task) {
    orderedTaskQueue.push(task);
    if (orderedTaskQueue.size() == 1) {
        Scheduler::insertTask(task);
    }
}

void OrderedTaskHandler::popOrderedTask() {
    if (orderedTaskQueue.empty()) {
        spdlog::warn("Can't pop from an empty queue");
    }
    else {
        orderedTaskQueue.pop();
        if (!orderedTaskQueue.empty()) {
            Scheduler::insertTask(orderedTaskQueue.front());
        }
    }
}

std::shared_ptr<Task> OrderedTaskHandler::frontOrderedTask() {
    if (!orderedTaskQueue.empty()) {
        return orderedTaskQueue.front();
    }
    else {
        spdlog::warn("Can't get front from an empty queue");
        return nullptr;
    }
}
