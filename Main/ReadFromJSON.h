#pragma once
#include <iostream>
#include <fstream>
#include  <nlohmann/json.hpp>
#include "Task.h"
#include "Scheduler.h"

using json = nlohmann::json; 

class ReadFromJSON
{
public:
    static void createTasksFromJSON(const string& filePath);
};

