#ifndef DEADLINE_H
#define DEADLINE_H

#include "Task.h"
#include <queue>
#include <vector>
#include <functional>
#include <ctime>
#include <memory> // for std::shared_ptr
#include "Consts.h"

using namespace std;

class DeadlineTask : public Task {
private:
    time_t deadline;

    // Min-heap to store DeadlineTask objects
    static priority_queue<DeadlineTask, vector<DeadlineTask>, greater<DeadlineTask>> minHeap;

public:
    DeadlineTask(int id, string priority, int runtime, time_t deadline);

    time_t getDeadline() const;

    // Comparator for the min-heap based on deadline
    bool operator>(const DeadlineTask& other) const;

    // Add a new DeadlineTask to the heap
    void addTask(const DeadlineTask& task);

    // Retrieve and remove the task with the earliest deadline
    DeadlineTask popEarliestTask();

    // Check if the heap is empty
    bool isEmpty() const;

    // Function to get the current size of the heap
    size_t getTaskCount() const;

    // Function that checks the time now with the time of the head of the heap,
    // if it's close, it passes it to the critical queue by changing the status to critical and calling the insertTask function
    static void deadlineMechanism();
};

#endif // DEADLINE_H