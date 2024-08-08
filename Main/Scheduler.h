#pragma once

#include "Task.h"
#include "RealTimeScheduler.h"
#include "WeightRoundRobinScheduler.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <string>

class Task;
class RealTimeScheduler;
class WeightRoundRobinScheduler;

class Scheduler
{
private:
    static RealTimeScheduler realTimeScheduler;
    static WeightRoundRobinScheduler wrrQueues;
    Task* Input();

public:
    void StartScheduling();
    Scheduler(const Scheduler& other) = delete;
    Scheduler& operator=(const Scheduler& other) = delete;
    Scheduler(Scheduler&& other) = delete;
    Scheduler& operator=(Scheduler&& other) = delete;

    void InsertTask();
    static int taskAmount;
    static void execute(Task* task);
    static void displayMessage(const Task* task);
    static void preemptive(Task* task);
};
