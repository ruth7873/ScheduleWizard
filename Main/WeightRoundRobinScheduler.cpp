#include "WeightRoundRobinScheduler.h"
#include "Consts.h"
#include "queue"
#include "Scheduler.h"
#include <thread>
#include <chrono>
using namespace std;

WeightRoundRobinScheduler::WeightRoundRobinScheduler() {
    WRRQueues[Consts::HIGHER] = Queue{ std::queue<Task*>(), Consts::HIGHER_WEIGHT };
    WRRQueues[Consts::MIDDLE] = Queue{ std::queue<Task*>(), Consts::MIDDLE_WEIGHT };
    WRRQueues[Consts::LOWER] = Queue{ std::queue<Task*>(), Consts::LOWER_WEIGHT };
}


void WeightRoundRobinScheduler::addTask(Task* task) {
    WRRQueues[task->getPriority()].queue.push(task);
}

std::unordered_map<std::string, Queue> WeightRoundRobinScheduler::getWrrQueues() {
    return WRRQueues;
}




void WeightRoundRobinScheduler::WeightRoundRobin()
{
    while (true) {
        int countTasks = 0;
        for (auto& pair : WRRQueues) {
            Queue* taskQueue = &pair.second;
            
            int weight = taskQueue->weight;
            int taskCountToRun = static_cast<int>(Scheduler::taskAmount * (weight / 100.0));
            std::cout << "taskCountToRun befor If: " << taskCountToRun << endl;
            taskCountToRun = (taskCountToRun == 0 && !taskQueue->queue.empty()) ? 1 : taskCountToRun;
            std::cout << "taskCountToRun after...: " << taskCountToRun << endl;

            cout << "Processing queue: " << pair.first << " with weight: " << weight << std::endl;


            while (!taskQueue->queue.empty() && countTasks < taskCountToRun) {
                cout << "size befor pop: " << taskQueue->queue.size() << endl;
                Task* task = taskQueue->queue.front();
                taskQueue->queue.pop();

                cout << "size after pop: " << taskQueue->queue.size() << endl;

                Scheduler::execute(task);

                countTasks++;
                Scheduler::taskAmount--;
                std::this_thread::sleep_for(std::chrono::seconds(1));

            }

            countTasks = 0; // Reset countTasks for the next queue
        }
    }
}
