#pragma once
#include <string>
#include <ctime>

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

    Task(int id, string priority, int runningTime, string status);
    Task(int, string priority, int runningTime);

    int getId() const;
    void setId(int newId);

    const string& getPriority() const;
    void setPriority(const string& newPriority);

    int getRunningTime() const;
    void setRunningTime(int newRunningTime);

    const string& getStatus() const;
    void setStatus(const string& newStatus);

    time_t getEntryTime() const;
    void setEntryTime(time_t newEntryTime);
};
