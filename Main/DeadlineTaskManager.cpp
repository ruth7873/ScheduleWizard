#include "DeadlineTaskManager.h"


// Definition of the static member
std::priority_queue<std::shared_ptr<DeadlineTask>, std::vector<std::shared_ptr<DeadlineTask>>, DeadlineTaskManager::CompareDeadline> DeadlineTaskManager::minHeap;

void DeadlineTaskManager::addTask(const std::shared_ptr<DeadlineTask>& task) {
	if (task->getPriority() != PrioritiesLevel::CRITICAL)
		minHeap.push(task);
}

std::shared_ptr<DeadlineTask> DeadlineTaskManager::getUpcomingTask() {
	if (minHeap.empty()) {
		cerr << "Heap is empty, can't pop\n";
		return nullptr;
	}
	return minHeap.top();
}


void DeadlineTaskManager::deadlineMechanism() {
	while (true) {
		if (!minHeap.empty()) {
			std::shared_ptr<DeadlineTask> earliestTask = minHeap.top();
			time_t currentTime = time(nullptr) * 100;
			// Check if the current time is close to the task's deadline
			if (currentTime >= earliestTask->getDeadline() - earliestTask->getRunningTime() &&
				earliestTask->getPriority() != PrioritiesLevel::CRITICAL &&
				earliestTask->getStatus() != TaskStatus::COMPLETED &&
				earliestTask->getStatus() != TaskStatus::TERMINATED) {

				// Change the status of the task
				earliestTask->setPriority(PrioritiesLevel::CRITICAL);

				// Insert the task into the scheduler
				Scheduler::insertTask(dynamic_pointer_cast<Task>(earliestTask));
				// Remove the task from the heap
				{
					std::unique_lock<std::mutex> lock(Scheduler::rtLock);  // Lock the rtLock
					if (!minHeap.empty())
						minHeap.pop();
				}
			}
			else {
				if (earliestTask->getPriority() == PrioritiesLevel::CRITICAL ||
					earliestTask->getStatus() == TaskStatus::COMPLETED)

					minHeap.pop();
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Check every second

	}
}
