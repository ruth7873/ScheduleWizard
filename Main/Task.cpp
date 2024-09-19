#include "Task.h"
#include "Consts.h"
#include "Scheduler.h"

Task::Task(int id, string priority, int runningTime, string status, bool isOrdered)
	: id(id), priority(priority), runningTime(runningTime), status(status), isOrdered(isOrdered), counter(Scheduler::tasksCounter)
{}

Task::Task(int id, string priority, int runningTime, bool isOrdered)
	: Task(id, priority, runningTime, TaskStatus::CREATION, isOrdered)
{}

Task::Task(const shared_ptr<Task>& other)
	: id(other->id), priority(other->priority), runningTime(other->runningTime), status(other->status), isOrdered(other->isOrdered) {}


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
	Utility::inviteDisplayMessage(this);
}

bool Task::getIsOrdered()const {
	return isOrdered;
}

int Task::getCounter() const {
	return counter;
}
