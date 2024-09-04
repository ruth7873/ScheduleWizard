#include "LongTaskHandler.h"
int LongTaskHandler::sumOfAllSeconds = 0;
int LongTaskHandler::numOfSeconds = 0;
double LongTaskHandler::AverageLength = 0.0;
mutex LongTaskHandler::longTaskMutex;

bool LongTaskHandler::haveToSuspendLongTask(shared_ptr<Task> task) {
	std::lock_guard<std::mutex> lock(longTaskMutex);
	//Scheduler::printAtomically("average is " +to_string(AverageLength) +"\n");
	if (Scheduler::totalRunningTask <= 1)//if there is only one task in the system
		return false;
	if (task->getPriority() == PrioritiesLevel::CRITICAL && Scheduler::getRealTimeScheduler().getRealTimeQueue().size() == 1)//there is one critical task - not suspend it!!
		return false;
	if (numOfSeconds > AverageLength)
		return true;
	return false;
}
void LongTaskHandler::stopLongTask(shared_ptr<Task> longTask) {
	longTask->setStatus(TaskStatus::SUSPENDED);
	spdlog::info("The long task with ID: {} is suspended and will continue later.", longTask->getId());
	Scheduler::popTaskFromItsQueue(longTask);
	Scheduler::addTaskToItsQueue(longTask);
}

void LongTaskHandler::calculateAverageLength() {
	std::lock_guard<std::mutex> lock(longTaskMutex);
	if (Scheduler::totalRunningTask)
		AverageLength = static_cast<double> (sumOfAllSeconds) / Scheduler::totalRunningTask;
	Scheduler::printAtomically("the average is: " + to_string(AverageLength) + "\n");
}


// Getters
int LongTaskHandler::getSumOfAllSeconds() {
	return sumOfAllSeconds;
}

int LongTaskHandler::getNumOfSeconds() {
	return numOfSeconds;
}

double LongTaskHandler::getAverageLength() {
	return AverageLength;
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
	AverageLength = value;
}
