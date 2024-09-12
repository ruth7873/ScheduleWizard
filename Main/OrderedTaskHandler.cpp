#include "OrderedTaskHandler.h"

void OrderedTaskHandler::addOrderedtask(shared_ptr<Task>& task) {
	orderedTaskQueue.push(task);
	if (orderedTaskQueue.size() == 1)
		Scheduler::insertTask(task);
}
void OrderedTaskHandler::popOrderedTask() {
	if (orderedTaskQueue.empty())
		spdlog::warn(Logger::LoggerWarn::CANT_POP);
	else
		orderedTaskQueue.pop();
	if (!orderedTaskQueue.empty()) {
		Scheduler::insertTask(orderedTaskQueue.front());
	}
}
shared_ptr<Task> OrderedTaskHandler::frontOrderedTask() {
	if (!orderedTaskQueue.empty())
		return orderedTaskQueue.front();
	spdlog::warn(Logger::LoggerWarn::CANT_FRONT);
	return nullptr;
}