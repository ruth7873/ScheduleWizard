#include "Scheduler.h"

#include <thread>

void Scheduler::StartScheduling() {
    try {
        // Create a thread for the InsertTask function
        std::thread insertTask_Thread(&Scheduler::InsertTask, this);

        // Create a thread for real-Time Scheduler
        std::thread RTScheduler_Thread([this]() {
            realTime.realTimeSchedulerFunction();
            });

        // Create a thread for WRR Scheduler
        std::thread WRRScheduler_Thread([this]() {
            WrrQueues.WRRScheduler();
            });

        insertTask_Thread.join();
        RTScheduler_Thread.join();
        WRRScheduler_Thread.join();
    }
    catch (const std::exception& ex) {
        // Handle any exceptions that might occur during thread creation
        std::cerr << "Error creating threads: " << ex.what() << std::endl;
    }
}


void Scheduler::InsertTask()
{
    for (int i = 1; i < 10; i++) {
        std::cout << "InsertTask tread:" << i << endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
