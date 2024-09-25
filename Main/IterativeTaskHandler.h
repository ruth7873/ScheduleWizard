#pragma once
#include <memory>
#include <queue>
#include <vector>
#include <functional>
#include <chrono>
#include "Scheduler.h"
#include "IterativeTask.h"


class Task;
class IterativeTask;

using namespace std;

class IterativeTaskHandler
{
private:
	struct CompareIterative {
		bool operator()(const std::shared_ptr<IterativeTask>& lhs, const std::shared_ptr<IterativeTask>& rhs) const {
			return lhs->IterativeTask::getWaitTime() > rhs->IterativeTask::getWaitTime(); // Min-heap based on waitTime
		}
	};
		std::priority_queue<std::shared_ptr<IterativeTask>, std::vector<std::shared_ptr<IterativeTask>>, CompareIterative> minHeap;

public:
	 void pushIterativeTask(shared_ptr<IterativeTask> iterativeTask);

	 shared_ptr<IterativeTask> popIterativeTask();

	 void checkTime();//check with the top of heap

	 bool isEmpty() {
		 return minHeap.empty();
	 }
	 const std::priority_queue<std::shared_ptr<IterativeTask>, std::vector<std::shared_ptr<IterativeTask>>, CompareIterative>& getMinHeap() const {
		 return minHeap;
	 }
};
