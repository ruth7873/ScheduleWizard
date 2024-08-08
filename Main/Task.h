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
<<<<<<< HEAD
    Task(int, string, int, string);

    Task(string, int);

    int getId() const {
        return id;
    }
=======
    Task(int id, string priority, int runningTime, string status);
    Task(int, string priority, int runningTime);
>>>>>>> 3e0ceb65378f524166931a6836efd8d2f791065a

    int getId() const;
    void setId(int newId);

    const string& getPriority() const;
    void setPriority(const string& newPriority);

    int getRunningTime() const;
    void setRunningTime(int newRunningTime);

    const string& getStatus() const;
    void setStatus(const string& newStatus);

<<<<<<< HEAD
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
=======
    time_t getEntryTime() const;
    void setEntryTime(time_t newEntryTime);
};
>>>>>>> 3e0ceb65378f524166931a6836efd8d2f791065a
