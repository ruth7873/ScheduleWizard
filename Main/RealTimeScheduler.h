
#pragma once
#include <queue>
#include "Task.h"
#include <iostream>
#include <thread>

class RealTimeScheduler
{
private:
    queue<Task*> realTime;

public:
    void realTimeSchedulerFunction() {
        for (int i = 1; i < 10; i++) {
            std::cout << "realTimeSchedulerFunction tread:" << i << endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

};
