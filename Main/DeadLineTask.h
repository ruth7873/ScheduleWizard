#pragma once
#include "Task.h"
#include <ctime>
#include <memory> // for std::shared_ptr

class Task;

class DeadlineTask : public Task {
private:
    time_t deadline;

public:
    DeadlineTask(Task basicTask, int deadLineTime);
    time_t getDeadline() const;

    // Comparator for the min-heap based on deadline
    bool operator>(const DeadlineTask& other) const;

    DeadlineTask() = default;
};

