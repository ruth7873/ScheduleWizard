#include "Scheduler.h"
#include "Deadline.h"


int main() {
    DeadlineTask rootTask(0, "Root", 0, time(nullptr)); // Root task to manage the heap

    // Create some DeadlineTask objects
    time_t now = time(nullptr);
    DeadlineTask task1(1, "High", 10, now + 5);   // 5 seconds from now
    DeadlineTask task2(2, "Low", 20, now + 10);   // 10 seconds from now
    DeadlineTask task3(3, "Medium", 5, now + 7); // 4 seconds from now

    // Add tasks to the heap within the root task
    shared_ptr<Task> taskPtr1 = make_shared<DeadlineTask>(task1);
    shared_ptr<Task> taskPtr2 = make_shared<DeadlineTask>(task2);
    shared_ptr<Task> taskPtr3 = make_shared<DeadlineTask>(task3);

    rootTask.addTask(task3);

    Scheduler::insertTask(taskPtr1);
    Scheduler::insertTask(taskPtr2);
    Scheduler::insertTask(taskPtr3);

    //rootTask.addTask(task1);
    //rootTask.addTask(task2);

    Scheduler s;
    s.init();
}


