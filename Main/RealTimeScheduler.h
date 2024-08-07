#pragma once
#include <queue>
#include "Task.h" // Include Task.h here

class RealTimeScheduler
{
private:
<<<<<<< HEAD
	queue<Task*> realTimeQueue;

=======
	std::queue<Task*> realTimeQueue;
>>>>>>> 3a297fc44559574e4fbe38969c6328f9872609ae
public:
	std::queue<Task*> getRealTimeQueue() const {
		return realTimeQueue;
	}

	void addTask(Task* task) {
		realTimeQueue.push(task);
	}
	void realTimeSchedulerFunction();
};
