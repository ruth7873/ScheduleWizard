#include "Scheduler.h"

int Scheduler::taskAmount = 0;
RealTimeScheduler Scheduler::realTimeScheduler;
WeightRoundRobinScheduler Scheduler::WrrQueues;

void Scheduler::execute(Task* task) {
	task->setStatus(Consts::RUNNING);
	while (true) {
		if (task->getPriority() != Consts::CRITICAL && !realTimeScheduler.getRealTimeQueue().empty())
		{
			preemptive(task);
			task = realTimeScheduler.getRealTimeQueue().front();
			realTimeScheduler.getRealTimeQueue().pop();
			task->setStatus(Consts::RUNNING);
		}
		if (task->getRunningTime() == 0) // the task is completed - finish running successfully.
		{
			task->setStatus(Consts::COMPLETED);
			break;
		}
		else {
			try
			{
				task->setRunningTime(task->getRunningTime() - 1); // the task is running
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
			catch (const std::exception& e)
			{
				task->setStatus(Consts::TERMINATED);
				std::cout << " " << e.what() << std::endl;
				break;
			}
		}
	}
}

void Scheduler::displayMessage(const Task* task) {
	std::cout << "task " << task->getId() << " is " << task->getStatus() << std::endl;
}

void Scheduler::preemptive(Task* task) {
	task->setStatus(Consts::SUSPENDED);
	WrrQueues.addTask(task);
}