#include "WeightRoundRobinScheduler.h"



WeightRoundRobinScheduler::WeightRoundRobinScheduler() {
    WRRQueues.emplace(Consts::HIGHER, Queue{ std::queue<Task*>(), Consts::HIGHER_WEIGHT });
    WRRQueues.emplace(Consts::MIDDLE, Queue{ std::queue<Task*>(), Consts::MIDDLE_WEIGHT });
    WRRQueues.emplace(Consts::LOWER, Queue{ std::queue<Task*>(), Consts::LOWER_WEIGHT });
}

void WeightRoundRobinScheduler::addTask(Task* newTask)
{
    WRRQueues[newTask->getPriority()].WRRQueue.push(newTask);
}

