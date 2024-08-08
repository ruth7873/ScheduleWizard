#include "RealTimeScheduler.h"
#include "Scheduler.h"

/**
	 * @brief Function that manages the execution of real-time tasks.
	 *
	 * This function runs in an infinite loop. It checks if the real-time queue is not empty, and if it is not, it retrieves the first task from the queue and passes it to the `Scheduler::execute` function for processing.
	 */
void RealTimeScheduler::realTimeSchedulerFunction() {
	cout << "start real-time Tread\n";
    while (true) {
        // Wait until the queue is not empty
        while (realTimeQueue.empty());

        // Get the first task from the queue
        Task* task = realTimeQueue.front();
        realTimeQueue.pop();

        // Process the task
        Scheduler::execute(task);
    }
}