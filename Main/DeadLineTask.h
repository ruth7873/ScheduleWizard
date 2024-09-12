#pragma once
#include "Task.h"
#include <ctime>
#include <memory> // for std::shared_ptr

class Task;
//class DeadLineTask;

class DeadLineTask : public Task {
private:
    time_t deadline;

public:
    DeadLineTask(Task basicTask, int deadLineTime);
    time_t getDeadline() const;

    // Comparator for the min-heap based on deadline
    bool operator>(const DeadLineTask& other) const;

    DeadLineTask() = default;
};