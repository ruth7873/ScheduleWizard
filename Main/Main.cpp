#include <iostream>
#include "Scheduler.h"
#include "WeightRoundRobinScheduler.h"
#include "Task.h"
#include "Consts.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"
#include <spdlog/sinks/rotating_file_sink.h>
#include "Logger.h"
using namespace std;


int main() {
    Scheduler s;
    s.init();
}
