#include "OrderedTaskHandler.h"

void OrderedTaskHandler::addOrderedtask(shared_ptr<Task>& task) {
	orderedTaskQueue.push(task);
	if (orderedTaskQueue.size() == 1)
		Scheduler::insertTask(task);
}
void OrderedTaskHandler::popOrderedTask() {
	if (orderedTaskQueue.empty())
		spdlog::warn("Can't pop empty queue");
	else
		orderedTaskQueue.pop();
	if (!orderedTaskQueue.empty()) {
		Scheduler::insertTask(orderedTaskQueue.front());
	}
}
shared_ptr<Task> OrderedTaskHandler::frontOrderedTask() {
	if (!orderedTaskQueue.empty())
		return orderedTaskQueue.front();
	spdlog::warn("can't front empty queue");
	return nullptr;
}