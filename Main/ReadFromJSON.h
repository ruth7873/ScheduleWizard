#pragma once
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Scheduler.h"

class ReadFromJSON
{
private:
	using json = nlohmann::json;

public:
    static void createTasksFromJSON(const string&);

    static void createTasksFromJSONWithDelay(const string& filePath, string message = "Waiting a long time");
};

