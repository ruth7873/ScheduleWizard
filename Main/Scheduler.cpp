#include "Scheduler.h"
#include "Consts.h"

int Scheduler::taskAmount = 0;

RealTimeScheduler Scheduler::realTimeScheduler;
WeightRoundRobinScheduler Scheduler::wrrQueues;

void Scheduler::execute(Task* task) {
	task->setStatus(Consts::RUNNING);
	displayMessage(task,"");
	while ((task->getPriority() == Consts::CRITICAL && !realTimeScheduler.getRealTimeQueue().empty()) || realTimeScheduler.getRealTimeQueue().empty()) {

		if (task->getRunningTime() == 0)//the task is completed - finish running successfully.
		{
			task->setStatus(Consts::COMPLETED);
			displayMessage(task,"");
			break;
		}
		else {
			try
			{
				task->setRunningTime(task->getRunningTime() - 1);//the task is running
				//std::this_thread::sleep_for(std::chrono::seconds(1));
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
		wrrQueues.addTask(task);
		displayMessage(task,"");
	}
	task = nullptr;
}

void Scheduler::displayMessage(const Task* task, string message) {
	cout << "task " << task->getId() << " is " << task->getStatus();
	if (message.length() > 0)
		cout << message;
	cout << endl;
}
