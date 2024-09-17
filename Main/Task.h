#pragma once
#include <string>
#include <ctime>
#include <chrono>
#include "Consts.h"
#include "Utility.h"


using namespace std;

class Task
{
private:
	int id;
	string priority;
	int runningTime;
	string status;
	bool isOrdered;
public:

	//c'tors
	Task(int id, string priority, int runningTime, string status, bool isOrdered = false);
	Task(int id, string priority, int runningTime, bool isOrdered = false);

	//getters & setters
	virtual int getId() const;

	virtual const string& getPriority() const;
	virtual void setPriority(const string& newPriority);

	virtual int getRunningTime() const;
	virtual void setRunningTime(int newRunningTime);

	virtual const string& getStatus() const;
	virtual void setStatus(const string& newStatus);
	
	virtual bool getIsOrdered()const;
};
