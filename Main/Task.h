#pragma once
#include <string>
#include <ctime>
#include <chrono>
#include "Scheduler.h"

using namespace std;

class Task
{
private:
	int id;
	string priority;
	int runningTime;
	string status;
	time_t entryTime;

public:
	//c'tors
	Task(int id, string priority, int runningTime, string status);
	Task(int, string priority, int runningTime);

	//getters & setters
	virtual int getId() const;
	virtual void setId(int newId);

	virtual const string& getPriority() const;
	virtual void setPriority(const string& newPriority);

	virtual int getRunningTime() const;
	virtual void setRunningTime(int newRunningTime);

	virtual const string& getStatus() const;
	virtual void setStatus(const string& newStatus);

	virtual time_t getEntryTime() const;
	virtual void setEntryTime(time_t newEntryTime);
};
