#pragma once
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Scheduler.h"
#include "TaskFactory.h"
#include "Consts.h"

class IReadFromJSON {
public:
    virtual ~IReadFromJSON() = default;
    virtual void createTasksFromJSON(const std::string& filePath) = 0;
    virtual void createTasksFromJSONWithDelay(const string& filePath, string message = "Waiting a long time") = 0;

};

class ReadFromJSON: public IReadFromJSON
{
private:
	using json = nlohmann::json;

public:
     void createTasksFromJSON(const string&) override;

     void createTasksFromJSONWithDelay(const string& filePath, string message = "Waiting a long time") override;
};

