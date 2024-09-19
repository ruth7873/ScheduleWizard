#pragma once

#include <iostream>
#include <string>
#include "Task.h"

class Task;
class TaskFactory;

class IUtility {
public:
    virtual ~IUtility() = default;
    virtual void insertTaskFromInput() = 0;
};

class Utility : public IUtility {
public:
    static int validateIntegerInput(const std::string& inputMessage, const std::string& variableName, bool allowNegative);
    static void displayInviteMessage(Task* task);
    static void checkTaskIds();

    void insertTaskFromInput() override;
};

