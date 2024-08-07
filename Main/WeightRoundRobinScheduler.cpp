#include "WeightRoundRobinScheduler.h"
#include <iostream>
#include <thread>
void WeightRoundRobinScheduler::WRRScheduler()
{
	for (int i = 1; i < 10; i++) {
		std::cout << "WRRScheduler tread:" << i << endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
