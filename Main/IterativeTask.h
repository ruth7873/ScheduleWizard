#pragma once
#include "Task.h"

class IterativeTask : public Task
{
private:
    int iterationsRemaining;
    int executionInterval;
    time_t waitTime;

public:
    IterativeTask(const Task& basictask, int iterationsRemaining, int executionInterval);

    int getIterationsRemaining() const {
        return iterationsRemaining;
    }

    void setIterationsRemaining(int iterations) {
        iterationsRemaining = iterations;
    }

    int getExecutionInterval() const {
        return executionInterval;
    }

    void dereaseIterationsRemaining() {
        iterationsRemaining--;
    }

    void setWaitTime(time_t newWaitTime) {
        waitTime = newWaitTime;
    }

    void addWaitTime(time_t secondsToAdd) {
        waitTime += secondsToAdd;
    }

    time_t getWaitTime() const {
        return waitTime;
    }
};
