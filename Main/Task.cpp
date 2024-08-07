#include "Task.h"
#include <chrono>

Task::Task(int id, string priority, int runningTime, string status)
    : id(id), priority(priority), runningTime(runningTime), status(status)
{
    auto currentTime = std::chrono::system_clock::now();
    entryTime = std::chrono::system_clock::to_time_t(currentTime);
}

Task::Task(string priority, int runningTime)
    : Task(-1, priority, runningTime, "Creation")
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
}

time_t Task::getEntryTime() const {
    return entryTime;
}

void Task::setEntryTime(time_t newEntryTime) {
    entryTime = newEntryTime;
}
