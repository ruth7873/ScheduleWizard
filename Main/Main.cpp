#include <iostream>
#include "Scheduler.h"
#include "WeightRoundRobinScheduler.h"
#include "Task.h"
#include "Consts.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"
#include <spdlog/sinks/rotating_file_sink.h>
#include "Utilities.h"
using namespace std;


int main() {
    initialize_logger();
    Scheduler s;
    s.init();
}
