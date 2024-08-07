#pragma once
#include <string>
#include <ctime>

class Scheduler;

class Task {
private:
    int id;
    std::string priority;
    int runningTime;
    std::string status;
    time_t entryTime;

public:
    Task(int, std::string, int, std::string);
    Task(std::string, int);

    int getId() const;
    void setId(int newId);
    const std::string& getPriority() const;
    void setPriority(const std::string& newPriority);
    int getRunningTime() const;
    void setRunningTime(int newRunningTime);
    const std::string& getStatus() const;
    void setStatus(const std::string& newStatus);
    time_t getEntryTime() const;
    void setEntryTime(time_t newEntryTime);
};
