#pragma once
#include <memory>
#include "Task.h"
#include <mutex>
#include "Scheduler.h"

class LongTaskHandler
{
	static std::mutex longTaskMutex;

	static int sumOfAllSeconds;
	static int numOfSeconds;
	static double averageLength;

public:
	static void stopLongTask(shared_ptr<Task>);
	static void calculateAverageLength();
	static bool haveToSuspendLongTask(shared_ptr<Task> task);

	// Getters
	static int getSumOfAllSeconds();
	static int getNumOfSeconds();
	static double getAverageLength();

	// Setters
	static void addSumOfAllSeconds(int value);
	static void increaseNumOfSeconds();
	static void setSumOfAllSeconds(int value);
	static void setNumOfSeconds(int value);
	static void setAverageLength(double value);
};

