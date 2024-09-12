#include "Task.h"
#include "Consts.h"

Task::Task(int id, string priority, int runningTime, string status, bool isOrdered)
	: id(id), priority(priority), runningTime(runningTime), status(status), isOrdered(isOrdered)
{
	auto currentTime = std::chrono::system_clock::now();
	// Convert the time point to std::time_t
	//entryTime = std::chrono::system_clock::to_time_t(currentTime);
}

Task::Task(int id, string priority, int runningTime, bool isOrdered)
	: Task(id, priority, runningTime, TaskStatus::CREATION, isOrdered)
{}

int Task::getId() const {
	return id;
}

const string& Task::getPriority() const {
	return priority;
}

void Task::setPriority(const string& newPriority) {
	priority = newPriority;
}

int Task::getRunningTime() const {
	return runningTime;
}

void Task::setRunningTime(int newRunningTime) {
	runningTime = newRunningTime;
}

const string& Task::getStatus() const {
	return status;
}

void Task::setStatus(const string& newStatus) {
	status = newStatus;
	//if(newStatus != TaskStatus::CREATION)
	//Scheduler::displayMessage(this);
}

bool Task::getIsOrdered()const {
	return isOrdered;
}
