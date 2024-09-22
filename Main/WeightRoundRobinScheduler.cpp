#include "WeightRoundRobinScheduler.h"

WeightRoundRobinScheduler::WeightRoundRobinScheduler() {
    WRRQueues[PrioritiesLevel::HIGHER] = Queue{ std::queue<std::shared_ptr<Task>>(), WeightPrecents::HIGHER_WEIGHT };
    WRRQueues[PrioritiesLevel::MIDDLE] = Queue{ std::queue<std::shared_ptr<Task>>(), WeightPrecents::MIDDLE_WEIGHT };
    WRRQueues[PrioritiesLevel::LOWER] = Queue{ std::queue<std::shared_ptr<Task>>(), WeightPrecents::LOWER_WEIGHT };
}

WeightRoundRobinScheduler::~WeightRoundRobinScheduler() {
    for (auto& pair : WRRQueues) {
        while (!pair.second.queue.empty()) {
            pair.second.queue.pop();
        }
    }
}

void WeightRoundRobinScheduler::addTask(std::shared_ptr<Task> task) {
    WRRQueues[task->getPriority()].queue.push(task);
    spdlog::info(Logger::LoggerInfo::ADD_NON_CRITICAL_TASK, task->getId(), task->getPriority());
}

std::unordered_map<std::string, Queue>& WeightRoundRobinScheduler::getWrrQueues() {
    return WRRQueues;
}

void WeightRoundRobinScheduler::weightRoundRobinFunction() {
    while (true) {
        for (auto& pair : WRRQueues) {
            Queue& taskQueue = pair.second;
            int weight = taskQueue.weight;
            int taskCountToRun = static_cast<int>(Scheduler::totalRunningTask * (weight / 100.0));
            taskCountToRun = (taskCountToRun == 0 && !taskQueue.queue.empty()) ? 1 : taskCountToRun;

            while (!taskQueue.queue.empty() && taskCountToRun > 0) {
                std::shared_ptr<Task> task = taskQueue.queue.front();
                //taskQueue.queue.pop();

                if (auto iterativeTask = std::dynamic_pointer_cast<IterativeTask>(task)) {
                    task->setStatus(TaskStatus::CREATION);
                }
                else {
                    while (task && (task->getPriority() == PrioritiesLevel::CRITICAL || task->getStatus() == TaskStatus::COMPLETED)) {
                        if (!taskQueue.queue.empty()) {
                            task = taskQueue.queue.front();
                            taskQueue.queue.pop();
                        }
                        else {
                            task = nullptr;
                        }
                    }
                }

                {
                    std::unique_lock<std::mutex> lock(Scheduler::rtLock);
                }

                if (task && task->getPriority() != PrioritiesLevel::CRITICAL && task->getStatus() != TaskStatus::COMPLETED) {
                    Scheduler::execute(task);
                }

                taskCountToRun--;
            }
        }
    }
}
