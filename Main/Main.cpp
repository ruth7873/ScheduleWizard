#include <iostream>
#include "Scheduler.h"
#include "WeightRoundRobinScheduler.h"
#include "Task.h"
#include "Consts.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"
using namespace std;


void initialize_logger() {
    // יצירת יומן יומי שמתחלף בחצות
    auto daily_logger = spdlog::daily_logger_mt("daily_logger", "logs/daily_log.txt", 0, 0);

    // הגדרת היומן שנוצר כברירת מחדל
    spdlog::set_default_logger(daily_logger);
    spdlog::set_level(spdlog::level::info); // הגדרת רמת היומן ל-info
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v"); // התאמת תבנית היומן
    spdlog::get("daily_logger")->info("Logger initialized and logging to daily file");
}
int main() {
    //Scheduler::taskAmount = 30;
    initialize_logger();

    Scheduler s;
    s.StartScheduling();

    //for (int i = 0; i <= 15; ++i) {
    //    Task* task = new Task(i, Consts::MIDDLE, 5, Consts::RUNNING);
    //    wrrScheduler.addTask(task);
    //}

    //// Add tasks to the WRR queues
    //for (int i = 16; i <= 20; ++i) {
    //    Task* task = new Task(i, Consts::HIGHER, 5, Consts::RUNNING);
    //    wrrScheduler.addTask(task);
    //}

    //for (int i = 21; i <= 30; ++i) {
    //    Task* task = new Task(i, Consts::LOWER, 5, Consts::RUNNING);
    //    wrrScheduler.addTask(task);
    //}

    //// Run the WeightRoundRobin scheduler
    //wrrScheduler.WeightRoundRobinFunction();
    //realTimeScheduler.realTimeSchedulerFunction();


}
