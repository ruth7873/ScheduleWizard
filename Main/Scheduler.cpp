#include "Scheduler.h"
#include "Task.h"

int Scheduler::taskAmount = 0;
RealTimeScheduler Scheduler::realTimeScheduler;
WeightRoundRobinScheduler Scheduler::wrrQueues;

void Scheduler::execute(Task* task) {
    task->setStatus(Consts::RUNNING);
	while (task->getRunningTime() > 0) {
		if (task->getPriority() != Consts::CRITICAL && !realTimeScheduler.getRealTimeQueue().empty())
		{
			preemptive(task);
			task = realTimeScheduler.getRealTimeQueue().front();
			realTimeScheduler.getRealTimeQueue().pop();
			task->setStatus(Consts::RUNNING);
		}	
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
  task->setStatus(Consts::COMPLETED);
  delete task;
}

void Scheduler::displayMessage(const Task* task) {
	std::cout << "task " << task->getId() << " is " << task->getStatus() << std::endl;
}

void Scheduler::preemptive(Task* task) {
	task->setStatus(Consts::SUSPENDED);
	wrrQueues.addTask(task);
}

void Scheduler::StartScheduling() {
    try {
        // Create a thread for the InsertTask function
        std::thread insertTask_Thread(&Scheduler::InsertTask, this);

        // Create a thread for real-Time Scheduler
        std::thread RTScheduler_Thread([this]() {
            realTimeScheduler.realTimeSchedulerFunction();
            });

        // Create a thread for WRR Scheduler
        std::thread WRRScheduler_Thread([this]() {
            cout << "start wrr scheduler tread\n";
            // wrrQueues.;
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

Task* Scheduler::Input()
{
    std::string priority;
    int runningTime;
    std::string input;

    std::cout << "Enter the priority for the task. Options: Critical, Higher, Middle, Lower: \n";
    std::cin >> priority;

    // Input validation for priority
    while (priority != Consts::CRITICAL && priority != Consts::HIGHER &&
        priority != Consts::MIDDLE && priority != Consts::LOWER) {
        std::cout << "Invalid priority. Please enter one of the specified options." << std::endl;
        std::cout << "Enter the priority for the task. Options: Critical, Higher, Middle, Lower: ";
        std::cin >> priority;
    }

    // Input validation for runningTime
    while (true) {
        std::cout << "Enter the task running time: \n";
        std::cin >> input;

        // Check if the input contains only digits
        if (input.find_first_not_of("0123456789-") != std::string::npos) {
            std::cerr << "Invalid input. Running time should contain only numeric digits." << std::endl;
            continue; // Restart the loop for a valid input
        }

        // Convert the input to an integer
        runningTime = std::stoi(input);

        if (runningTime < 0) {
            std::cerr << "Invalid input: Running time cannot be negative." << std::endl;
            continue; // Restart the loop for a valid input
        }

        break; // If runningTime is valid, exit the loop
    }

    // Assuming other fields like status and entryTime are set elsewhere
    return new Task(taskAmount++, priority, runningTime);
}

void Scheduler::InsertTask()
{
    cout << "start insert tread\n";
    while (true) {
        Task* newTask = Input(); // Get a new task from input
        if (newTask == nullptr) {
            std::cerr << "Error: Invalid task input. Please try again." << std::endl;
            continue; // Skip the rest of the loop iteration if input is invalid
        }

        if (newTask->getPriority() == Consts::CRITICAL) {
            realTimeScheduler.addTask(newTask); // Add task to real-time scheduler for real-time tasks
        }
        else {
            wrrQueues.addTask(newTask); // Add task to Weighted Round Robin scheduler for non-real-time tasks
        }
       // std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}
