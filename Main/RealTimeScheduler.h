#pragma once
#include <queue>
#include "Scheduler.h"

class RealTimeScheduler
{

private:
<<<<<<< HEAD

	queue<Task*> realTimeQueue;

public:
	queue<Task*> getRealTimeQueue() const {
		return realTimeQueue;
	}
=======
    std::queue<Task*> realTimeQueue;

public:
    std::queue<Task*> getRealTimeQueue() const {
        return realTimeQueue;
    }
>>>>>>> 3e0ceb65378f524166931a6836efd8d2f791065a

    void addTask(Task* task) {
        realTimeQueue.push(task);
    }
<<<<<<< HEAD
=======

>>>>>>> 3e0ceb65378f524166931a6836efd8d2f791065a
    void realTimeSchedulerFunction();
};
