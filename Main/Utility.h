#pragma once
#include <iostream>
#include <string>
#include "Task.h" 

using namespace std;

class Task;

class IUtility {
public:
	virtual ~IUtility() = default;
	virtual void insertTaskFromInput() = 0;


};

class Utility: public IUtility
{
public:
	static int integerValidation(string inputMassege, string variableName, bool negative);
	static void inviteDisplayMessage(Task* task);
	void insertTaskFromInput() override;
};

