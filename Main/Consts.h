#pragma once
#include <string>
using namespace std;

class TaskStatus {
public:
	static const string CREATION;
	static const string COMPLETED;
	static const string SUSPENDED;
	static const string TERMINATED;
	static const string RUNNING;
};

class PrioritiesLevel {
public:
	static const string CRITICAL;
	static const string HIGHER;
	static const string MIDDLE;
	static const string LOWER;
};

class WeightPrecents {
public:
	static const int HIGHER_WEIGHT;
	static const int MIDDLE_WEIGHT;
	static const int LOWER_WEIGHT;

};
class Scenario {
public:
	static const string SCENARIO_1_FILE_PATH;
	static const string SCENARIO_2_FILE_PATH;
	static const string SCENARIO_3_FILE_PATH;
	static const string SCENARIO_4_FILE_PATH;
	static const string SCENARIO_5_FILE_PATH;
	static const string SCENARIO_6_FILE_PATH;
	static const string SCENARIO_7_FILE_PATH;
	static const string SCENARIO_8_FILE_PATH;
	static const string SCENARIO_9_FILE_PATH;
	static const string SCENARIO_10_FILE_PATH;


};
