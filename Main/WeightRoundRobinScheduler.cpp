#include "WeightRoundRobinScheduler.h"
#include "Consts.h"
#include "queue"
#include "Scheduler.h"
#include <thread>
#include <chrono>
using namespace std;

<<<<<<< HEAD
/**
 * @brief Constructor for WeightRoundRobinScheduler.
 *
 * Initializes the WRRQueues map with three priority levels: HIGHER, MIDDLE, and LOWER.
 * Each queue is associated with a weight defined in the Consts header.
 */
WeightRoundRobinScheduler::WeightRoundRobinScheduler() {
    WRRQueues[Consts::HIGHER] = Queue{ std::queue<Task*>(), Consts::HIGHER_WEIGHT };
    WRRQueues[Consts::MIDDLE] = Queue{ std::queue<Task*>(), Consts::MIDDLE_WEIGHT };
    WRRQueues[Consts::LOWER] = Queue{ std::queue<Task*>(), Consts::LOWER_WEIGHT };
}


/**
 * @brief Destructor for WeightRoundRobinScheduler.
 *
 * Cleans up all dynamically allocated Task objects in the WRRQueues map.
 * This ensures that no memory leaks occur when the scheduler is destroyed.
 */
WeightRoundRobinScheduler::~WeightRoundRobinScheduler() {
    for (auto& pair : WRRQueues) {
        while (!pair.second.queue.empty()) {
            delete pair.second.queue.front();
            pair.second.queue.pop();
        }
    }
}

/**
 * @brief Adds a task to the appropriate queue based on its priority.
 *
 * @param task A pointer to the Task object to be added to the queue.
 *
 * The task is pushed into the queue corresponding to its priority (HIGHER, MIDDLE, LOWER).
 */
void WeightRoundRobinScheduler::addTask(Task* task) {
    WRRQueues[task->getPriority()].queue.push(task);
}

std::unordered_map<std::string, Queue> WeightRoundRobinScheduler::getWrrQueues() {
    return WRRQueues;
}



/**
 * @brief Executes tasks in the queues using the Weighted Round Robin scheduling algorithm.
 *
 * This function continuously iterates through the WRRQueues map, executing tasks based on their queue's weight.
 * For each queue, it calculates the number of tasks to run proportionally to the queue's weight.
 * Real-time delays (sleep_for) are introduced between task executions.
 *
 * The function handles tasks in a fair and efficient manner, ensuring that higher-priority tasks are given more processing time.
 */
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
=======
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
>>>>>>> 3e0ceb65378f524166931a6836efd8d2f791065a
    }
}
