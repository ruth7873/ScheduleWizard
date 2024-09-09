#pragma once
#include "Task.h"
#include "DeadlineTaskManager.h"
#include <ctime>
#include <memory> // for std::shared_ptr

class Task;
class DeadlineTaskManager;

class DeadLineTask : public Task {
private:
    time_t deadline;
    // Static instance of DeadlineTaskManager
    static DeadlineTaskManager manager;
public:
    DeadLineTask(Task basicTask, int deadLineTime);
    time_t getDeadline() const;

    // Comparator for the min-heap based on deadline
    bool operator>(const DeadLineTask& other) const;

    DeadLineTask() = default;
};
