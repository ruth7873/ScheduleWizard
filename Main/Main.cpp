#include <iostream>
#include "Scheduler.h"
#include "WeightRoundRobinScheduler.h"
#include "Task.h"
#include "Consts.h"

using namespace std;

int main() {
    Scheduler::taskAmount = 30;
    WeightRoundRobinScheduler wrrScheduler;

    for (int i = 0; i <= 15; ++i) {
        Task* task = new Task(i, Consts::MIDDLE, 5, Consts::RUNNING);
        wrrScheduler.addTask(task);
    }

    // Add tasks to the WRR queues
    for (int i = 16; i <= 20; ++i) {
        Task* task = new Task(i, Consts::HIGHER, 5, Consts::RUNNING);
        wrrScheduler.addTask(task);
    }

    for (int i = 21; i <= 30; ++i) {
        Task* task = new Task(i, Consts::LOWER, 5, Consts::RUNNING);
        wrrScheduler.addTask(task);
    }

    // Run the WeightRoundRobin scheduler
    wrrScheduler.WeightRoundRobin();

    return 0;
}
