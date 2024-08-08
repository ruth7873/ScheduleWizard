#pragma once
#include <iostream>
#include <thread>
#include <chrono>    
#include <string>
#include "Task.h"
#include "RealTimeScheduler.h"
#include "WeightRoundRobinScheduler.h"
<<<<<<< HEAD
  

class RealTimeScheduler;
=======
#include <iostream>
#include <thread>
#include <chrono>
#include <string>

class Task;
class RealTimeScheduler;
class WeightRoundRobinScheduler;
>>>>>>> 3e0ceb65378f524166931a6836efd8d2f791065a

class Scheduler
{
private:
<<<<<<< HEAD

	static RealTimeScheduler realTimeScheduler;
	static WeightRoundRobinScheduler wrrQueues;
	Task* Input();

public:

	void StartScheduling();
	void InsertTask();
	static int taskAmount;
 	static void execute(Task* task);
	static void displayMessage(const Task* task, string);
	static void preemptive(Task* task);

=======
    static RealTimeScheduler realTimeScheduler;
    static WeightRoundRobinScheduler wrrQueues;
    Task* Input();

public:
    void StartScheduling();
    void InsertTask();
    static int taskAmount;
    static void execute(Task* task);
    static void displayMessage(const Task* task);
    static void preemptive(Task* task);
>>>>>>> 3e0ceb65378f524166931a6836efd8d2f791065a
};
