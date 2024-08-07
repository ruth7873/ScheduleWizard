#include "RealTimeScheduler.h"
#include "Scheduler.h"

void RealTimeScheduler::realTimeSchedulerFunction() {
    cout << "start real-time Tread\n";
    while (true) {
        while (realTimeQueue.empty());
        Task* task = RealTimeScheduler::realTimeQueue.front();
        RealTimeScheduler::realTimeQueue.pop();
        cout << "start exe task " << task->getId() << endl;
        Scheduler::execute(task);
    }
}