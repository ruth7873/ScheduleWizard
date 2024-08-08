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

/**
 * @class Scheduler
 * @brief Manages task scheduling and execution.
 *
 * The Scheduler class provides functionalities for task execution, task insertion, and scheduling. It handles tasks based on their priority and uses multiple schedulers to manage execution.
 */
class Scheduler
{
private:
    // Real-time scheduler instance for managing real-time tasks
    static RealTimeScheduler realTimeScheduler;

    // Weighted Round Robin scheduler instance for managing non-real-time tasks
    static WeightRoundRobinScheduler wrrQueues;

    /**
     * @brief Reads input from the user and creates a new task.
     *
     * This method prompts the user to input the priority and running time for a new task. It validates the input and creates a Task object accordingly.
     *
     * @return A pointer to the newly created Task object.
     */
    Task* Input();

public:
    // Static variable to keep track of the number of tasks created
    static int taskAmount;

    /**
     * @brief Starts the scheduling process by creating and managing threads.
     *
     * This method initializes threads for task insertion, real-time scheduling, and weighted round-robin scheduling, and ensures they run concurrently.
     */
    void StartScheduling();

    /**
     * @brief Inserts a new task into the appropriate scheduler.
     *
     * This method continually prompts the user to input new tasks and adds them to the real-time scheduler if they are critical, or to the weighted round-robin scheduler otherwise.
     */
    void InsertTask();

    /**
     * @brief Executes a given task.
     *
     * This static method processes the provided task, updating its status and handling any preemption if needed. It also manages exceptions during task execution.
     *
     * @param task Pointer to the Task object to be executed.
     */
    static void execute(Task* task);

    /**
     * @brief Displays the status of a task.
     *
     * This static method prints the current status of the specified task to the standard output.
     *
     * @param task Pointer to the Task object whose status is to be displayed.
     */
    static void displayMessage(const Task* task);

    /**
     * @brief Handles task preemption.
     *
     * This static method sets the status of the given task to suspended and adds it to the weighted round-robin scheduler queue. This is used to preempt the current task when a higher priority task needs to be processed.
     *
     * @param task Pointer to the Task object to be preempted.
     */
    static void preemptive(Task* task);
};