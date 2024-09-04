#pragma once
#include <queue>
#include <memory>
#include <mutex>
#include "Task.h"
#include "Scheduler.h"
using namespace std;

class Scheduler; // Forward declaration
class Task;
/**
 * @class RealTimeScheduler
 * @brief Manages real-time tasks.
 *
 * This class is responsible for managing tasks that need to be executed in real-time. It maintains a queue of tasks and processes them according to their order.
 */
class RealTimeScheduler
{
private:
	queue<shared_ptr<Task>> realTimeQueue;

public:
	//c'rots, d'tors
	RealTimeScheduler() {}
	RealTimeScheduler(const RealTimeScheduler& rt) = delete;
	RealTimeScheduler(RealTimeScheduler&& rt) = delete;
	~RealTimeScheduler();


    void realTimeSchedulerFunction();

	queue<shared_ptr<Task>>& getRealTimeQueue();
	void addTask(shared_ptr<Task> task);
};
