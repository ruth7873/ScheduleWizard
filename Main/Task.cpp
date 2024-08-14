
#include "Task.h"
#include <chrono>
#include "Scheduler.h"

Task::Task(int id, string priority, int runningTime, string status)
    : id(id), priority(priority), runningTime(runningTime), status(status)
{
    auto currentTime = std::chrono::system_clock::now();
    // Convert the time point to std::time_t
    entryTime = std::chrono::system_clock::to_time_t(currentTime);
}

Task::Task(int id, string priority, int runningTime)
    : Task(id, priority, runningTime, Consts::CREATION)
{}

int Task::getId() const {
    return id;
}

void Task::setId(int newId) {
    id = newId;
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
    Scheduler::displayMessage(this);
}

time_t Task::getEntryTime() const {
    return entryTime;
}

void Task::setEntryTime(time_t newEntryTime) {
    entryTime = newEntryTime;
}
