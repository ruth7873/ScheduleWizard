#include "Scheduler.h"

int Scheduler::totalRunningTask = 0;
unsigned int Scheduler::taskIds = 0;
unsigned int Scheduler::tasksCounter = 0;

std::mutex Scheduler::coutMutex;
std::mutex Scheduler::rtLock;
std::mutex Scheduler::realTimeQueueMutex;
std::mutex Scheduler::wrrQueueMutex;

RealTimeScheduler Scheduler::realTimeScheduler;
WeightRoundRobinScheduler Scheduler::wrrQueuesScheduler;
IterativeTaskHandler Scheduler::iterativeTaskHandler;
DeadlineTaskManager Scheduler::deadlineTaskManager;
OrderedTaskHandler Scheduler::orderedTaskHandler;


Scheduler::Scheduler(IReadFromJSON* reader, IUtility* utilities)
    : reader(reader),
    utilities(utilities) {}

queue <shared_ptr<Task>> Scheduler::starvationCheckQueue;

const int Scheduler::STARVATION = 10;



void Scheduler::init() {
    Logger::initialize_logger();

    spdlog::info(Logger::LoggerInfo::START_SCHEDULER);
    try {
        std::thread readtasksFromJSONThread([this]() {
            SetThreadDescription(GetCurrentThread(), L"createTasksFromJSON");
            spdlog::info(Logger::LoggerInfo::START_READ_FROM_JSON_THREAD);
            reader->createTasksFromJSON(Scenario::SCENARIO_1_FILE_PATH);
        }); 
        std::thread insertTaskThread([this]() {
            SetThreadDescription(GetCurrentThread(), L"InsertTask");
            spdlog::info(Logger::LoggerInfo::START_THREAD, "InsertTask");
            utilities->insertTaskFromInput();
            });

        std::thread RTSchedulerThread([this]() {
            SetThreadDescription(GetCurrentThread(), L"RealTimeScheduler");
            spdlog::info(Logger::LoggerInfo::START_THREAD, "RealTimeScheduler");
            realTimeScheduler.realTimeSchedulerFunction();
            });

        std::thread WRRSchedulerThread([this]() {
            SetThreadDescription(GetCurrentThread(), L"WeightRoundRobinScheduler");
            spdlog::info(Logger::LoggerInfo::START_THREAD, "WeightRoundRobinScheduler");
            wrrQueuesScheduler.weightRoundRobinFunction();
            });

        std::thread IterativeTaskHandlerThread([this]() {
            SetThreadDescription(GetCurrentThread(), L"IterativeTaskHandler");
            spdlog::info(Logger::LoggerInfo::START_THREAD, "IterativeTaskHandler");
            iterativeTaskHandler.checkTime();
            });
      
        // Create a thread for Iterative task manager
        std::thread CheckStarvation_Thread([this]() {
          SetThreadDescription(GetCurrentThread(), L"CheckStarvation");
          spdlog::info(Logger::LoggerInfo::START_THREAD, "CheckStarvation");
          this->checkStarvation();
          });
      
        readtasksFromJSONThread.join();
        insertTaskThread.join();
        RTSchedulerThread.join();
        WRRSchedulerThread.join();
        IterativeTaskHandlerThread.join();
		    CheckStarvation_Thread.join();
      }
      catch (const std::exception& ex) {
        // Handle any exceptions that might occur during thread creation
        spdlog::error(Logger::LoggerError::ERROR_CREATE_THREAD, ex.what());
      }
}


void Scheduler::insertTask(std::shared_ptr<Task> newTask) {
    if (newTask == nullptr) {
        std::cerr << "Error: Invalid task input. Please try again." << std::endl;
        spdlog::error("Error: Invalid task input. Skipping task insertion.");
    }
    else if (newTask->getIsOrdered() && orderedTaskHandler.frontOrderedTask() != newTask) {
        orderedTaskHandler.addOrderedTask(newTask);
    }
    else {
        addTaskToItsQueue(newTask);
        totalRunningTask++;
        LongTaskHandler::addSumOfAllSeconds(newTask->getRunningTime());

        if (auto iterativeTask = std::dynamic_pointer_cast<IterativeTask>(newTask)) {
            iterativeTaskHandler.pushIterativeTask(iterativeTask);
        }
        else if (auto deadlineTask = std::dynamic_pointer_cast<DeadlineTask>(newTask)) {
            deadlineTaskManager.addTask(deadlineTask);
        }
    }
}

void Scheduler::addTaskToItsQueue(std::shared_ptr<Task> taskToAdd) {
    if (taskToAdd->getPriority() == PrioritiesLevel::CRITICAL) {
        std::lock_guard<std::mutex> lock(realTimeQueueMutex);
        realTimeScheduler.addTask(taskToAdd);
        spdlog::info(Logger::LoggerInfo::ADD_CRITICAL_TASK, taskToAdd->getId());
    }
    else {
        std::lock_guard<std::mutex> lock(wrrQueueMutex);
        wrrQueuesScheduler.addTask(taskToAdd);
        spdlog::info(Logger::LoggerInfo::ADD_NON_CRITICAL_TASK, taskToAdd->getId(), taskToAdd->getPriority());
      	starvationCheckQueue.push(taskToAdd);
    }
}

void Scheduler::popTaskFromItsQueue(std::shared_ptr<Task> taskToPop) {
    if (taskToPop->getPriority() == PrioritiesLevel::CRITICAL) {
        std::lock_guard<std::mutex> lock(realTimeQueueMutex);
        if (!realTimeScheduler.getRealTimeQueue().empty()) {
            realTimeScheduler.getRealTimeQueue().pop();
        }
    }
    else {
        std::lock_guard<std::mutex> lock(wrrQueueMutex);
        if (!wrrQueuesScheduler.getWrrQueues()[taskToPop->getPriority()].queue.empty()) {
            wrrQueuesScheduler.getWrrQueues()[taskToPop->getPriority()].queue.pop();
        }
    }
}

void Scheduler::execute(std::shared_ptr<Task> task) {
    deadlineTaskManager.deadlineMechanism();
    LongTaskHandler::calculateAverageLength();
    LongTaskHandler::setNumOfSeconds(0);
    spdlog::info(Logger::LoggerInfo::START_EXECUTE, task->getId());
    task->setStatus(TaskStatus::RUNNING);

    while (true) {
        if (task->getRunningTime() == 0) {
            task->setStatus(TaskStatus::COMPLETED);
            popTaskFromItsQueue(task);
            totalRunningTask--;
            spdlog::info(Logger::LoggerInfo::TASK_COMPLITED, task->getId());
            if (task->getIsOrdered()) {
                orderedTaskHandler.popOrderedTask();
            }
            break;
        }
        if (LongTaskHandler::haveToSuspendLongTask(task)) {
            LongTaskHandler::stopLongTask(task);
            break;
        }
        if (task->getPriority() != PrioritiesLevel::CRITICAL && !realTimeScheduler.getRealTimeQueue().empty()) {
            spdlog::info(Logger::LoggerInfo::TASK_PREEMPTITVE, task->getId());
            preemptive(task);
            return;
        }

        try {
            task->setRunningTime(task->getRunningTime() - 1);
            LongTaskHandler::increaseNumOfSeconds();
            LongTaskHandler::addSumOfAllSeconds(-1);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        catch (const std::exception& e) {
            spdlog::error(Logger::LoggerError::TASK_TERMINATED, task->getId(), e.what());
            task->setStatus(TaskStatus::TERMINATED);
            popTaskFromItsQueue(task);
            totalRunningTask--;
            LongTaskHandler::addSumOfAllSeconds(task->getRunningTime() * -1);
            break;
        }
    }
}
    
void Scheduler::checkStarvation() {
	while (true) {
		// Wait until the mutex is released
		{
			std::unique_lock<std::mutex> lock(Scheduler::rtLock);  // Lock the rtLock
		}// The mutex will be automatically released at the end of this scope

		if (!starvationCheckQueue.empty()) {
			if (starvationCheckQueue.front()->getStatus() != TaskStatus::CREATION) {
				starvationCheckQueue.pop();
			}
			else if (starvationCheckQueue.front()->getStatus() == TaskStatus::CREATION && tasksCounter - starvationCheckQueue.front()->getCounter() >= STARVATION) {
				spdlog::error("there is starvation!!");
				std::stringstream errorMsg;
				errorMsg << "Starvation detected! Task ID: " << starvationCheckQueue.front()->getId();
				throw std::runtime_error(errorMsg.str());
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			tasksCounter++;
		}
			else
				tasksCounter = 0;
	}
}
      
void Scheduler::preemptive(std::shared_ptr<Task> task) {
    task->setStatus(TaskStatus::SUSPENDED);
    spdlog::info(Logger::LoggerInfo::TASK_SUSPENDED, task->getId());
}

void Scheduler::displayMessage(const Task* task) {
    printAtomically("task " + std::to_string(task->getId()) + " with priority: " + task->getPriority() + " and running time " + std::to_string(task->getRunningTime()) + " is " + task->getStatus() + "\n");
}

void Scheduler::printAtomically(const std::string& message) {
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << message;
}

RealTimeScheduler& Scheduler::getRealTimeScheduler() {
    return realTimeScheduler;
}

WeightRoundRobinScheduler& Scheduler::getWrrQueuesScheduler() {
    return wrrQueuesScheduler;
}

IterativeTaskHandler& Scheduler::getIterativeTaskHandler() {
    return iterativeTaskHandler;
}

DeadlineTaskManager& Scheduler::getDeadlineTaskManager() {
    return deadlineTaskManager;
}

OrderedTaskHandler& Scheduler::getOrderedTaskHandler() {
    return orderedTaskHandler;
}
queue<shared_ptr<Task>>& Scheduler::getStarvationCheckQueue() {
	return starvationCheckQueue;
}
void Scheduler::setStarvationCheckQueue(queue<shared_ptr<Task>> queue) {
	starvationCheckQueue = queue;
}
int Scheduler::getSTARVATION() {
	return STARVATION;
}