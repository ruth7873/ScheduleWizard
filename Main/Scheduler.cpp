#include "Scheduler.h"
#include "Consts.h"
#include "Task.h"
#include "ReadFromJSON.h"


int Scheduler::taskAmount = 0;
int Scheduler::taskCounter = 0;
mutex Scheduler::rtLock;
RealTimeScheduler Scheduler::realTimeScheduler;
WeightRoundRobinScheduler Scheduler::wrrQueues;

/**
 * @brief Executes a given task.
 *
 * This function processes a task by running it and updating its status. If the task is not critical and there are tasks in the real-time scheduler queue, the current task is preempted and a task from the real-time queue is executed. The function also handles exceptions by setting the task status to terminated and printing an error message.
 *
 * @param task Pointer to the task to be executed.
 */
void Scheduler::execute(Task* task) {
    spdlog::info("Executing task with ID: {}", task->getId());
	task->setStatus(Consts::RUNNING);
    // Continue executing the task while it has remaining running time
    while (task->getRunningTime() > 0) {
        if (task->getPriority() != Consts::CRITICAL && !realTimeScheduler.getRealTimeQueue().empty()) {
            spdlog::info("Preempting task with ID: {} for real-time task.", task->getId());
            preemptive(task);
            return;
        }
        try {
            // Simulate task execution by decrementing running time
            task->setRunningTime(task->getRunningTime() - 1); 
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        catch (const std::exception& e) {
            // Handle any exceptions that occur during execution
            spdlog::error("Exception occurred while executing task with ID: {}: {}", task->getId(), e.what());
            task->setStatus(Consts::TERMINATED);
            break; // Exit the loop if an exception is caught
        }
    }

    // Set the task status to COMPLETED when execution is finished
    task->setStatus(Consts::COMPLETED);
    if (task->getPriority() == Consts::CRITICAL) {
        realTimeScheduler.getRealTimeQueue().pop();
    }
    else {
        wrrQueues.getWrrQueues()[task->getPriority()].queue.pop();
    }
  
    spdlog::info("Task with ID: {} completed.", task->getId());
    if (task != nullptr) {
        delete task;
    }
}

/**
 * @brief Displays the status of a task.
 *
 * This function prints the current status of the specified task to the standard output.
 *
 * @param task Pointer to the task whose status is to be displayed.
 */
void Scheduler::displayMessage(const Task* task) {
    std::cout << "task " << task->getId() <<" with priority: " << task->getPriority()<< " is " << task->getStatus() << std::endl;
}

/**
 * @brief Handles task preemption.
 *
 * This function sets the status of the given task to suspended and adds it to the weighted round-robin queue. This is used to preempt the current task when a higher priority task needs to be processed.
 *
 * @param task Pointer to the task to be preempted.
 */
void Scheduler::preemptive(Task* task) {
    task->setStatus(Consts::SUSPENDED);
    spdlog::info("Task with ID: {} suspended and added back to WRR queue.", task->getId());
}



/**
 * @brief Initiates the scheduling process by creating and managing threads for various scheduler functions.
 *
 * This function creates separate threads for inserting tasks, real-time scheduling, and Weighted Round Robin scheduling.
 * It utilizes threading to allow concurrent execution of these tasks in the Scheduler class context.
 * Exceptions that may occur during thread creation are caught and handled within the function.
 */
void Scheduler::StartScheduling() {
    spdlog::info("Starting scheduling process.");
    try {
        //ReadFromJSON::createTasksFromJSON(Consts::SCENARIO_1_FILE_PATH);
        
        std::thread readtasksFromJSON_Thread([this]() {
            spdlog::info("read tasks From JSON thread started.");
            ReadFromJSON::createTasksFromJSONWithDelay(Consts::SCENARIO_1_FILE_PATH, 3, 30);
            });

        // Create a thread for the InsertTask function
        std::thread insertTask_Thread(&Scheduler::InsertTaskFromInput, this);
        spdlog::info("InsertTask thread started.");

        // Create a thread for real-Time Scheduler
        std::thread RTScheduler_Thread([this]() {
            spdlog::info("RealTimeScheduler thread started.");
            realTimeScheduler.realTimeSchedulerFunction();
            });

        // Create a thread for WRR Scheduler
        std::thread WRRScheduler_Thread([this]() {
            spdlog::info("WeightRoundRobinScheduler thread started.");
            wrrQueues.WeightRoundRobinFunction();
            });

        insertTask_Thread.join();
        RTScheduler_Thread.join();
        WRRScheduler_Thread.join();
    }
    catch (const std::exception& ex) {
        // Handle any exceptions that might occur during thread creation
        spdlog::error("Error creating threads: {}", ex.what());

    }
}

void Scheduler::InsertTaskFromInput()
{
    while (true) {
        InsertTask(Input());
    }
}



/**
 * @brief This function allows the user to input details for a new task, including priority and running time.
 *
 * @return A pointer to the newly created Task object based on the user input.
 */
Task* Scheduler::Input()
{
    std::string priority;
    int runningTime;
    std::string input;

    std::cout<< "Enter the priority for the task. Options: Critical, Higher, Middle, Lower: \n";
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
    spdlog::info("New task created with priority: {} and running time: {}", priority, runningTime);
    // Assuming other fields like status and entryTime are set elsewhere
    taskAmount++;
    return new Task(taskCounter++, priority, runningTime);
}


/**
 * @brief Continuously prompts the user to input task details and inserts the tasks into the appropriate scheduler.
 *
 * This function loops indefinitely, prompting the user to input task details using the Input function.
 * If the input task is valid, it is added to the corresponding scheduler based on its priority level.
 * Tasks with Critical priority are added to the real-time scheduler, while others are added to the Weighted Round Robin scheduler.
 */
void Scheduler::InsertTask(Task* newTask)
{
    if (newTask == nullptr) {
        std::cerr << "Error: Invalid task input. Please try again." << std::endl; 
        spdlog::error("Error: Invalid task input. Skipping task insertion.");
    }
    if (newTask->getPriority() == Consts::CRITICAL) {
        realTimeScheduler.addTask(newTask); // Add task to real-time scheduler for real-time tasks
        spdlog::info("Critical task with ID: {} added to RealTimeScheduler.", newTask->getId());
    }
    else {
        wrrQueues.addTask(newTask); // Add task to Weighted Round Robin scheduler for non-real-time tasks
        spdlog::info("Non-critical task with ID: {} added to WRR queue.", newTask->getId());
    }
    
}
