#include "RealTimeScheduler.h"
#include "Scheduler.h"

void RealTimeScheduler::realTimeSchedulerFunction() {
<<<<<<< HEAD
	while (true) {
		while (realTimeQueue.empty());
		Task* task = RealTimeScheduler::realTimeQueue.front();
		RealTimeScheduler::realTimeQueue.pop();
		Scheduler::execute(task);
	}
=======
    cout << "start real-time Tread\n";
    while (true) {
        while (realTimeQueue.empty());
        Task* task = RealTimeScheduler::realTimeQueue.front();
        RealTimeScheduler::realTimeQueue.pop();
        cout << "start exe task " << task->getId() << endl;
        Scheduler::execute(task);
    }
>>>>>>> 3e0ceb65378f524166931a6836efd8d2f791065a
}