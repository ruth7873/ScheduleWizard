#include "WeightRoundRobinScheduler.h"
#include "Consts.h"
#include "queue"
#include "Scheduler.h"
#include <thread>
#include <chrono>
#include <memory>
#include "IterativeTask.h"


using namespace std;

/**
 * @brief Constructor for WeightRoundRobinScheduler.
 *
 * Initializes the WRRQueues map with three priority levels: HIGHER, MIDDLE, and LOWER.
 * Each queue is associated with a weight defined in the Consts header.
 */
WeightRoundRobinScheduler::WeightRoundRobinScheduler() {
    WRRQueues[PrioritiesLevel::HIGHER] = Queue{ std::queue<shared_ptr<Task>>(), WeightPrecents::HIGHER_WEIGHT };
    WRRQueues[PrioritiesLevel::MIDDLE] = Queue{ std::queue<shared_ptr<Task>>(), WeightPrecents::MIDDLE_WEIGHT };
    WRRQueues[PrioritiesLevel::LOWER] = Queue{ std::queue<shared_ptr<Task>>(), WeightPrecents::LOWER_WEIGHT };

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
void WeightRoundRobinScheduler::addTask(shared_ptr<Task> task) {
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
	while (true) {  // Infinite loop to continuously process tasks
		int countTasks = 0;

		for (auto& pair : WRRQueues) {  // Iterate through each priority queue
			Queue* taskQueue = &pair.second;

			int weight = taskQueue->weight;
			// Calculate the number of tasks to run based on the queue's weight
			int taskCountToRun = static_cast<int>(Scheduler::totalRunningTask * (weight / 100.0));

			// Ensure at least one task runs if the queue is not empty
			taskCountToRun = (taskCountToRun == 0 && !taskQueue->queue.empty()) ? 1 : taskCountToRun;

			while (!taskQueue->queue.empty() && countTasks < taskCountToRun) {
				shared_ptr<Task> task = taskQueue->queue.front();

				if (shared_ptr<IterativeTask> iterativeTask = dynamic_pointer_cast<IterativeTask>(task)) {
					// Check if dynamic_pointer_cast succeeded
					if (iterativeTask){
						task->setStatus(TaskStatus::CREATION);
					}
				}
				else {
					// check if the task is not a deadline task that removed to Critical Q 
					while (task != nullptr && (task->getPriority() == PrioritiesLevel::CRITICAL || task->getStatus() == TaskStatus::COMPLETED)) {
						if (!taskQueue->queue.empty()) {
							taskQueue->queue.pop();
						}
						if (!taskQueue->queue.empty())
						{
							shared_ptr<Task> task = taskQueue->queue.front();
						}
						else {
							task = nullptr;
						}
					}
				}

				// Wait until the mutex is released
				{
					std::unique_lock<std::mutex> lock(Scheduler::rtLock);  // Lock the rtLock
				}// The mutex will be automatically released at the end of this scope

				// Check if the task is not null and execute it
				if (task != nullptr && task->getPriority() != PrioritiesLevel::CRITICAL &&task->getStatus() != TaskStatus::COMPLETED) {
					Scheduler::execute(task);
				}
				countTasks++;
			}
			countTasks = 0;  // Reset the task count for the next queue
		}
	}
}
