#include "WeightRoundRobinScheduler.h"

WeightRoundRobinScheduler::WeightRoundRobinScheduler() {
    // Initialization code if needed
}

void WeightRoundRobinScheduler::addTask(Task* task) {
    // Determine the task's priority and add it to the corresponding queue
    std::string priority = task->getPriority();

    if (priority == Consts::HIGHER) {
        WRRQueues[Consts::HIGHER].WRRQueue.push(task);
    }
    else if (priority == Consts::MIDDLE) {
        WRRQueues[Consts::MIDDLE].WRRQueue.push(task);
    }
    else if (priority == Consts::LOWER) {
        WRRQueues[Consts::LOWER].WRRQueue.push(task);
    }
}
