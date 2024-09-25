#pragma once
// cppcheck-suppress missingIncludeSystem
#include <string>

class PrioritiesLevel {
public:
    static const std::string CRITICAL;
    static const std::string HIGHER;
    static const std::string LOWER;
    static const std::string MIDDLE;
};

class Scenario {
public:
    static const std::string SCENARIO_1_FILE_PATH;
    static const std::string SCENARIO_10_FILE_PATH;
    static const std::string SCENARIO_11_FILE_PATH;
    static const std::string SCENARIO_2_FILE_PATH;
    static const std::string SCENARIO_3_FILE_PATH;
    static const std::string SCENARIO_4_FILE_PATH;
    static const std::string SCENARIO_5_FILE_PATH;
    static const std::string SCENARIO_6_FILE_PATH;
    static const std::string SCENARIO_7_FILE_PATH;
    static const std::string SCENARIO_8_FILE_PATH;
    static const std::string SCENARIO_9_FILE_PATH;
};

class TaskStatus {
public:
    static const std::string COMPLETED;
    static const std::string CREATION;
    static const std::string RUNNING;
    static const std::string SUSPENDED;
    static const std::string TERMINATED;
};

class TaskType {
public:
    static const std::string BASIC;
    static const std::string DEAD_LINE;
    static const std::string ITERATIVE;
    static const std::string ORDERED;
};

class WeightPrecents {
public:
    static constexpr int HIGHER_WEIGHT = 50;
    static constexpr int LOWER_WEIGHT = 10;
    static constexpr int MIDDLE_WEIGHT = 30;
};
