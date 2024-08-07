#include <iostream>
#include "Scheduler.h"
#include "WeightRoundRobinScheduler.h"
#include "Task.h"
#include "Consts.h"

using namespace std;

int main() {
    Scheduler::taskAmount = 10;
    WeightRoundRobinScheduler wrrScheduler;

    // Add tasks to the WRR queues
    for (int i = 1; i <= 5; ++i) {
        Task* task = new Task(i, Consts::HIGHER, 5, Consts::RUNNING);
        wrrScheduler.addTask(task);
    }
    for (int i = 6; i <= 10; ++i) {
        Task* task = new Task(i, Consts::MIDDLE, 5, Consts::RUNNING);
        wrrScheduler.addTask(task);
    }
    for (int i = 11; i <= 15; ++i) {
        Task* task = new Task(i, Consts::LOWER, 5, Consts::RUNNING);
        wrrScheduler.addTask(task);
    }

    // Run the WeightRoundRobin scheduler
    wrrScheduler.WeightRoundRobin();

    return 0;
}
