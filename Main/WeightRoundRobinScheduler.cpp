#include "WeightRoundRobinScheduler.h"
#include "Consts.h"
#include "queue"
#include "Scheduler.h"
using namespace std;

unordered_map<string, Queue> WeightRoundRobinScheduler::WRRQueues;
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
            // pair.first is the name (string)
            // pair.second is the Q (Queue)

            string nameQueue = pair.first;
            Queue taskQueue = pair.second;
            
            int weight = taskQueue.weight;
            int taskCountToRun = static_cast<int>(Scheduler::taskAmount * (weight / 100.0));
            taskCountToRun = (taskCountToRun == 0 && !taskQueue.queue.empty()) ? 1 : taskCountToRun;

            cout << "Processing queue: " << nameQueue << " with weight: " << weight << std::endl;


            while (!taskQueue.queue.empty() && countTasks < taskCountToRun) {
                Task* task = taskQueue.queue.front();
                taskQueue.queue.pop();
                Scheduler::execute(task);
                countTasks++;
            }

            countTasks = 0; // Reset countTasks for the next queue
        }

        // Sleep to prevent busy-waiting (optional)
        this_thread::sleep_for(chrono::seconds(1));

            // Process each queue
  /*          while (!taskQueue.queue.empty()) {
                if (weight <= countTasks ) {
                    countTasks = 0;
                    break;
                }
                Task* task = taskQueue.queue.front();
                Scheduler::execute(task);
                countTasks++;
            }*/
       // }

    }
}
