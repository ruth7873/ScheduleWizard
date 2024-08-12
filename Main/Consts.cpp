#include "Consts.h"

const string TaskStatus::COMPLETED = "Completed";
const string TaskStatus::SUSPENDED = "Suspended";
const string TaskStatus::TERMINATED = "Terminated";
const string TaskStatus::RUNNING = "Running";
const string TaskStatus::CREATION = "Creation";


const string PrioritiesLevel::CRITICAL = "Critical";
const string PrioritiesLevel::HIGHER = "Higher";
const string PrioritiesLevel::MIDDLE = "Middle";
const string PrioritiesLevel::LOWER = "Lower";

const int WeightPrecents::LOWER_WEIGHT = 10;
const int WeightPrecents::MIDDLE_WEIGHT = 30;
const int WeightPrecents::HIGHER_WEIGHT = 50;