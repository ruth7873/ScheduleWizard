
#include "Task.h"
#include <chrono>

Task::Task(int id, string priority, int runningTime, string status)
    : id(id), priority(priority), runningTime(runningTime), status(status)
{
    auto currentTime = std::chrono::system_clock::now();

    // Convert the time point to std::time_t
    entryTime = std::chrono::system_clock::to_time_t(currentTime);
}

Task::Task(string priority, int runningTime)
    : Task(-1, priority, runningTime, "Creation")
{}
