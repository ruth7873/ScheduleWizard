#include "Scheduler.h"

RealTimeScheduler Scheduler::realTimeScheduler;
WeightRoundRobinScheduler Scheduler::WrrQueues;
Task* Scheduler::currentTask = nullptr;
void Scheduler::execute(Task* task) {
	currentTask = task;
	while (realTimeScheduler.getRealTimeQueue().empty()) {

		if (task->getRunningTime() == 0)//the task is completed - finish running successfully.
		{
			task->setStatus(Consts::COMPLETED);
			break;
		}
		else {
			try
			{
				task->setRunningTime(task->getRunningTime() - 1);//the task is running
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
			catch (const std::exception& e)
			{
				task->setStatus(Consts::TERMINATED);
				displayMessage(task, e.what());
				break;
			}
		}
	}//real-time task arrived or task comleted.
	if (task->getStatus() != Consts::COMPLETED && task->getStatus() != Consts::TERMINATED) {
		task->setStatus(Consts::SUSPENDED);//real time task arrived- hijack.
		WrrQueues.addTask(task);
	}
	currentTask = nullptr;
}