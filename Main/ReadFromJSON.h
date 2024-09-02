#pragma once
#include <iostream>
#include <fstream>
#include  <nlohmann/json.hpp>
#include "Task.h"
#include "Scheduler.h"
#include "Timer.h"


class ReadFromJSON
{
private:
    using json = nlohmann::json;

public:
    static void createTasksFromJSON(const string&);

    static void createTasksFromJSONWithDelay(const string& filePath, string message = "Waiting a long time");

};

