#include "LongTaskHandler.h"

std::mutex LongTaskHandler::longTaskMutex;
int LongTaskHandler::numOfSeconds = 0;
int LongTaskHandler::sumOfAllSeconds = 0;
double LongTaskHandler::averageLength = 0.0;

bool LongTaskHandler::haveToSuspendLongTask(std::shared_ptr<Task> task) {
	std::lock_guard<std::mutex> lock(longTaskMutex);

	if (Scheduler::totalRunningTask <= 1)
		return false;

	if (task->getPriority() == PrioritiesLevel::CRITICAL &&
		Scheduler::getRealTimeScheduler().getRealTimeQueue().size() == 1)
		return false;
	if (Scheduler::getWrrQueuesScheduler().getWrrQueues()[task->getPriority()].queue.size() <= 1)
		return false;
	return numOfSeconds > averageLength;
}

void LongTaskHandler::stopLongTask(std::shared_ptr<Task> longTask) {
	longTask->setStatus(TaskStatus::SUSPENDED);
	spdlog::info(Logger::LoggerInfo::LONG_TASK_SUSPENDED, longTask->getId(), longTask->getPriority());

	Scheduler::popTaskFromItsQueue(longTask);
	Scheduler::addTaskToItsQueue(longTask);
}

void LongTaskHandler::calculateAverageLength() {
	std::lock_guard<std::mutex> lock(longTaskMutex);

	if (Scheduler::totalRunningTask != 0)
		averageLength = static_cast<double>(sumOfAllSeconds) / Scheduler::totalRunningTask;

	Scheduler::printAtomically("The average is: " + std::to_string(averageLength) + "\n");
}

// Getters
int LongTaskHandler::getSumOfAllSeconds() {
	return sumOfAllSeconds;
}

int LongTaskHandler::getNumOfSeconds() {
	return numOfSeconds;
}

double LongTaskHandler::getAverageLength() {
	return averageLength;
}

// Setters
void LongTaskHandler::addSumOfAllSeconds(int value) {
	sumOfAllSeconds += value;
}

void LongTaskHandler::increaseNumOfSeconds() {
	numOfSeconds++;
}

void LongTaskHandler::setSumOfAllSeconds(int value) {
	sumOfAllSeconds = value;
}

void LongTaskHandler::setNumOfSeconds(int value) {
	numOfSeconds = value;
}

void LongTaskHandler::setAverageLength(double value) {
	averageLength = value;
}
