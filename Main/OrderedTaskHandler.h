#pragma once
#include <memory>
#include "Task.h"
class OrderedTaskHandler
{
	queue<shared_ptr<Task>> orderedTaskQueue;
public:
	void addOrderedtask(shared_ptr<Task>&);
	void popOrderedTask();
	shared_ptr<Task> frontOrderedTask();
};

