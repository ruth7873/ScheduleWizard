#include "Scheduler.h"
#include "Consts.h"
#include <thread>

int Scheduler::taskAmount = 0;

RealTimeScheduler Scheduler::realTime;

WeightRoundRobinScheduler Scheduler::wrrQueues;

Task* Scheduler::currentTask = nullptr;



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

Task* Scheduler::Input()
{
    std::string priority;
    int runningTime;
    std::string input;

    std::cout << "Enter the priority for the task. Options: Critical, Higher, Middle, Lower: ";
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
        std::cout << "Enter the task running time: ";
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
    return new Task(priority, runningTime);
}


void Scheduler::InsertTask()
{
    while (true) {
        Task* newTask = Input(); // Get a new task from input
        if (newTask == nullptr) {
            std::cerr << "Error: Invalid task input. Please try again." << std::endl;
            continue; // Skip the rest of the loop iteration if input is invalid
        }

        std::cout << "Building new task" << std::endl;
        newTask->setId(taskAmount++); // Assign a unique ID to the task

        if (newTask->getPriority() == Consts::CRITICAL) {
            realTime.addTask(newTask); // Add task to real-time scheduler for real-time tasks
        }
        else {
            wrrQueues.addTask(newTask); // Add task to Weighted Round Robin scheduler for non-real-time tasks
        }
    }
}
