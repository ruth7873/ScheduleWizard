#include "WeightRoundRobinScheduler.h"
#include "Consts.h"
#include "queue"
#include "Scheduler.h"
#include <thread>
#include <chrono>
#include "Timer.h"
using namespace std;

/**
 * @brief Constructor for WeightRoundRobinScheduler.
 *
 * Initializes the WRRQueues map with three priority levels: HIGHER, MIDDLE, and LOWER.
 * Each queue is associated with a weight defined in the Consts header.
 */
WeightRoundRobinScheduler::WeightRoundRobinScheduler() {
    WRRQueues[PrioritiesLevel::HIGHER] = Queue{ std::queue<Task*>(), WeightPrecents::HIGHER_WEIGHT };
    WRRQueues[PrioritiesLevel::MIDDLE] = Queue{ std::queue<Task*>(), WeightPrecents::MIDDLE_WEIGHT };
    WRRQueues[PrioritiesLevel::LOWER] = Queue{ std::queue<Task*>(), WeightPrecents::LOWER_WEIGHT };
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
    spdlog::info(Logger::LoggerInfo::ADD_NON_CRITICAL_TASK, task->getId(), task->getPriority());
}

std::unordered_map<std::string, Queue>& WeightRoundRobinScheduler::getWrrQueues() {
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
void WeightRoundRobinScheduler::weightRoundRobinFunction()
{
    
    while (true) {
        int countTasks = 0;
        for (auto& pair : WRRQueues) {
            Queue* taskQueue = &pair.second;

            int weight = taskQueue->weight;
         
            int taskCountToRun = static_cast<int>(Scheduler::totalRunningTask  * (weight / 100.0));

            taskCountToRun = (taskCountToRun == 0 && !taskQueue->queue.empty()) ? 1 : taskCountToRun;

            while (!taskQueue->queue.empty() && countTasks < taskCountToRun) {
                Task* task = taskQueue->queue.front();
          
            auto startTime = std::chrono::steady_clock::now();
            while (!Scheduler::rtLock.try_lock()) {
                    checkLoopTimeout(startTime, 300, "There are many critical tasks, it may cause starvation of other tasks");  // ����� �� ����� ������ �� Timeout �� 5 ����   
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));  // ����� busy-wait
            }
                    
            Scheduler::rtLock.unlock();
            if(task != nullptr){
                Scheduler::execute(task);
                }


                while (taskQueue->queue.front()->getStatus() == TaskStatus::RUNNING || taskQueue->queue.front()->getStatus()==TaskStatus::SUSPENDED) {
                    if(taskQueue->queue.front()->getStatus() == TaskStatus::RUNNING)
                        checkLoopTimeout(startTime, 300, "The task is running for too long");
                    else if(taskQueue->queue.front()->getStatus() == TaskStatus::SUSPENDED)
                        checkLoopTimeout(startTime, 300, "The task is suspended for too long");
                }
               countTasks++;

            }
            countTasks = 0; // Reset countTasksInCurrentQueue for the next queue
        }
    }
}