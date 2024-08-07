#include "WeightRoundRobinScheduler.h"
#include "Consts.h"
#include "queue"

unordered_map<string, Queue> WeightRoundRobinScheduler::WRRQueues;
WeightRoundRobinScheduler::WeightRoundRobinScheduler() {
    WRRQueues[Consts::HIGHER] = Queue{ std::queue<Task*>(), Consts::HIGHER_WEIGHT };
    WRRQueues[Consts::MIDDLE] = Queue{ std::queue<Task*>(), Consts::MIDDLE_WEIGHT };
    WRRQueues[Consts::LOWER] = Queue{ std::queue<Task*>(), Consts::LOWER_WEIGHT };
}

void WeightRoundRobinScheduler::WeightRoundRobin()
{
    while (true) {
        int countTasks = 0;
        for (auto& pair : WRRQueues) {
            // pair.first is the name (string)
            // pair.second is the Q (Queue)

            std::string nameQueue = pair.first;
            Queue taskQueue = pair.second;
            double weight = taskQueue.queue.size() * (taskQueue.weight / 100);
            // Process each queue
            while (!taskQueue.queue.empty()) {
                if (weight >= countTasks ) {
                    countTasks = 0;
                    break;
                }
                Task* task = taskQueue.queue.front();
                // Execute(task);
                countTasks++;
            }
        }

    }
}
