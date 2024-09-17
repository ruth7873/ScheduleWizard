#pragma once
#include "Task.h"

class Task;

class IterativeTask : public Task
{
private:
    int iterationsRemaining;
    int executionInterval;
    time_t waitTime;
    int runTime;

public:
    IterativeTask(const Task& basictask, int iterationsRemaining, int executionInterval);

    int getRunTime() const {
        return runTime;
    }

    /*void setRunTime(int runTime) {
        runTime = runTime;
    }*/

    int getIterationsRemaining() const {
        return iterationsRemaining;
    }

    //void setIterationsRemaining(int iterations) {
    //    iterationsRemaining = iterations;
    //}

    int getExecutionInterval() const {
        return executionInterval;
    }

    void decreaseIterationsRemaining() {
        iterationsRemaining--;
    }

    void setWaitTime(time_t newWaitTime) {
        waitTime = newWaitTime;
    }

 /*   void addWaitTime(time_t secondsToAdd) {
        waitTime += secondsToAdd;
    }*/

    time_t getWaitTime() const {
        return waitTime;
    }
};
