
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