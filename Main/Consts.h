
#pragma once
#include <string>
using namespace std;
class Consts
{
public:
	static const string CREATION;
	static const string COMPLETED;
	static const string SUSPENDED;
	static const string TERMINATED;
	static const string RUNNING;

	static const string CRITICAL;
	static const string HIGHER;
	static const string MIDDLE;
	static const string LOWER;

	static const int HIGHER_WEIGHT;
	static const int MIDDLE_WEIGHT;
	static const int LOWER_WEIGHT;

	static const string SCENARIO_1_FILE_PATH;
};