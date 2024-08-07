#include "Scheduler.h"
#include "Consts.h"

int Scheduler::taskAmount = 0;

RealTimeScheduler Scheduler::realTimeScheduler;
WeightRoundRobinScheduler Scheduler::WrrQueues;
Task* Scheduler::currentTask = nullptr;
void Scheduler::execute(Task* task) {
	currentTask = task;
	task->setStatus(Consts::RUNNING);
	displayMessage(task);
	while ((currentTask->getPriority() == Consts::CRITICAL && !realTimeScheduler.getRealTimeQueue().empty()) || realTimeScheduler.getRealTimeQueue().empty()) {

		if (task->getRunningTime() == 0)//the task is completed - finish running successfully.
		{
			task->setStatus(Consts::COMPLETED);
			displayMessage(task);
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
		displayMessage(task);
	}
	currentTask = nullptr;
}
void Scheduler::displayMessage(Task* task, string message) {
	cout << "task " << task->getId() << " is " << task->getStatus();
	if (message.length() > 0)
		cout << message;
	cout << endl;
}


//Task* Scheduler::Input()
//{
//    std::string priority;
//    int runningTime;
//    std::string input;
//
//    std::cout << "Enter the priority for the task. Options: Critical, Higher, Middle, Lower: ";
//    std::cin >> priority;
//
//    // Input validation for priority
//    while (priority != Consts::CRITICAL && priority != Consts::HIGHER &&
//        priority != Consts::MIDDLE && priority != Consts::LOWER) {
//        std::cout << "Invalid priority. Please enter one of the specified options." << std::endl;
//        std::cout << "Enter the priority for the task. Options: Critical, Higher, Middle, Lower: ";
//        std::cin >> priority;
//    }
//
//    // Input validation for runningTime
//    while (true) {
//        std::cout << "Enter the task running time: ";
//        std::cin >> input;
//
//        // Check if the input contains only digits
//        if (input.find_first_not_of("0123456789-") != std::string::npos) {
//            std::cerr << "Invalid input. Running time should contain only numeric digits." << std::endl;
//            continue; // Restart the loop for a valid input
//        }
//
//        // Convert the input to an integer
//        runningTime = std::stoi(input);
//
//        if (runningTime < 0) {
//            std::cerr << "Invalid input: Running time cannot be negative." << std::endl;
//            continue; // Restart the loop for a valid input
//        }
//
//        break; // If runningTime is valid, exit the loop
//    }
//
//    // Assuming other fields like status and entryTime are set elsewhere
//    return new Task(priority, runningTime);
//}
//
//
//void Scheduler::InsertTask()
//{
//    while (true) {
//        Task* newTask = Input(); // Get a new task from input
//        if (newTask == nullptr) {
//            std::cerr << "Error: Invalid task input. Please try again." << std::endl;
//            continue; // Skip the rest of the loop iteration if input is invalid
//        }
//
//        std::cout << "Building new task" << std::endl;
//        newTask->setId(taskAmount++); // Assign a unique ID to the task
//
//        if (newTask->getPriority() == Consts::CRITICAL) {
//            realTimeScheduler.addTask(newTask); // Add task to real-time scheduler for real-time tasks
//        }
//        else {
//            WrrQueues.addTask(newTask); // Add task to Weighted Round Robin scheduler for non-real-time tasks
//            break;
//        }
//    }
//}

//void Scheduler::rt() {
//	execute(WrrQueues.getWrrQueues()[Consts::LOWER].WRRQueues.front());
//	std::this_thread::sleep_for(std::chrono::seconds(5));
//	Scheduler::realTimeScheduler.realTimeSchedulerFunction();
//}