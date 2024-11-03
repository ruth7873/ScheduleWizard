#include "IterativeTaskHandler.h"

void IterativeTaskHandler::pushIterativeTask(std::shared_ptr<IterativeTask> iterativeTask)
{
    int iterationsRemaining = iterativeTask->getIterationsRemaining();
    if (iterationsRemaining <= 1) {
        return;
    }

    iterativeTask->decreaseIterationsRemaining();


    auto currentTime = std::chrono::system_clock::now();
    auto waitTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds(iterativeTask->getExecutionInterval()) + currentTime.time_since_epoch()).count();
    iterativeTask->setWaitTime(waitTime);

    minHeap.emplace(iterativeTask);
    spdlog::info(Logger::LoggerInfo::PUSH_ITERATIVE_TASK_TO_HEAP, iterativeTask->getId(), iterativeTask->getIterationsRemaining(), iterativeTask->getPriority());
}

std::shared_ptr<IterativeTask> IterativeTaskHandler::popIterativeTask()
{
    if (isEmpty()) {
        cerr << "Heap is empty, can't pop\n";
        return nullptr;
    }
    
    std::shared_ptr<IterativeTask> topTask = minHeap.top();
    minHeap.pop();
    spdlog::info(Logger::LoggerInfo::POP_ITERATIVE_TASK_FROM_HEAP, topTask->getId());
    topTask->setRunningTime(topTask->getRunTime());

    return topTask;
}

void IterativeTaskHandler::checkTime()
{
    while (true) {
        if (!isEmpty()) {
            auto currentTime = std::chrono::system_clock::now();
            auto currentTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()).count();
            auto waitTime = minHeap.top()->getWaitTime();

            if (currentTimeMs >= waitTime) {
                auto poppedTask = popIterativeTask();
                Task task(std::dynamic_pointer_cast<Task>(poppedTask));
                task.setStatus(TaskStatus::CREATION);
                Scheduler::insertTask(std::make_shared<Task>(task));
                pushIterativeTask(poppedTask);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Check every second
    }
}
