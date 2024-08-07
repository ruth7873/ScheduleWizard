#pragma once
#include <string>
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
    Task(int, string, int, string);

    Task(string, int);

    int getId() const {
        return id;
    }

    void setId(int newId) {
        id = newId;
    }

    const string& getPriority() const {
        return priority;
    }

    void setPriority(const string& newPriority) {
        priority = newPriority;
    }

    int getRunningTime() const {
        return runningTime;
    }

    void setRunningTime(int newRunningTime) {
        runningTime = newRunningTime;
    }

    const string& getStatus() const {
        return status;
    }

    void setStatus(const string& newStatus) {
        status = newStatus;
    }

    time_t getEntryTime() const {
        return entryTime;
    }

    void setEntryTime(time_t newEntryTime) {
        entryTime = newEntryTime;
    }
};
