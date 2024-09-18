#pragma once
#include <thread>
#include <chrono>    
#include <string>
#include <memory>
#include <mutex>
#include <iostream>
#include "Task.h"
#include "Logger.h"
#include "Consts.h"
#include "Utility.h"
#include "ReadFromJSON.h"
#include "RealTimeScheduler.h"
#include "DeadlineTaskManager.h"
#include "IterativeTaskHandler.h"
#include "WeightRoundRobinScheduler.h"
#include "OrderedTaskHandler.h"
#include "LongTaskHandler.h"

class RealTimeScheduler;
class WeightRoundRobinScheduler;
class DeadlineTaskManager;
class IterativeTaskHandler;
class OrderedTaskHandler;
class IReadFromJSON;
class ReadFromJSON;
class IUtility;
class Utility;

class Scheduler
{
private:
	static RealTimeScheduler realTimeScheduler;
	static IterativeTaskHandler iterativeTaskHandler;
	static WeightRoundRobinScheduler wrrQueuesScheduler;
	static DeadlineTaskManager deadlineTaskManager;
	static OrderedTaskHandler orderedTaskHandler;
	static std::mutex coutMutex;
	static std::mutex realTimeQueueMutex;
	static std::mutex wrrQueueMutex;

	IReadFromJSON* reader;
	IUtility* utilies;

	static queue <shared_ptr<Task>> starvationCheckQueue;
	static const int STARVATION;
public:
	static const unsigned int MAX_TASKS = std::numeric_limits<unsigned int>::max();
	static std::mutex rtLock;
	static int totalRunningTask;
	static unsigned int taskIds;

	static unsigned int tasksCounter;

	Scheduler(IReadFromJSON* reader, IUtility* utilies);
	void init();

	static void printAtomically(const std::string& message);
	static void insertTask(std::shared_ptr<Task> newTask);
	static void execute(std::shared_ptr<Task> task);
	static void displayMessage(const Task* task);
	static void preemptive(std::shared_ptr<Task> task);

	static void popTaskFromItsQueue(std::shared_ptr<Task> taskToPop);
	static void addTaskToItsQueue(std::shared_ptr<Task> taskToAdd);
	void checkStarvation();

	static RealTimeScheduler& getRealTimeScheduler();
	static WeightRoundRobinScheduler& getWrrQueuesScheduler();
	static IterativeTaskHandler& getIterativeTaskHandler();
	static DeadlineTaskManager& getDeadlineTaskManager();
	static OrderedTaskHandler& getOrderedTaskHandler();

	static queue<shared_ptr<Task>>& getStarvationCheckQueue();
	static void setStarvationCheckQueue(queue<shared_ptr<Task>>);

	static int getSTARVATION();
};

