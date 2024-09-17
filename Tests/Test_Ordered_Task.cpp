<<<<<<< HEAD
//#include "doctest.h"
//#include "../Main/Scheduler.h"
//#include "Helper.h"
//
//
//TEST_CASE("OrderedTaskHandler operations") {
//    Scheduler s(new ReadFromJSON(), new Utility());
//    PopAllTheQueue(s);
//
//    SUBCASE("Adding tasks") {
//        auto task1 = std::make_shared<Task>(1,PrioritiesLevel::HIGHER, 10, true);
//        auto task2 = std::make_shared<Task>(2,PrioritiesLevel::HIGHER, 20, true);
//
//        s.getOrderedTaskHandler().addOrderedtask(task1);
//        CHECK(s.getOrderedTaskHandler().frontOrderedTask() == task1);
//        CHECK(s.getWrrQueuesScheduler().getWrrQueues()[task1->getPriority()].queue.size() == 1);
//        CHECK(s.getWrrQueuesScheduler().getWrrQueues()[task1->getPriority()].queue.front() == task1);
//
//        s.getOrderedTaskHandler().addOrderedtask(task2);
//        CHECK(s.getOrderedTaskHandler().frontOrderedTask() == task1);
//        CHECK(s.getWrrQueuesScheduler().getWrrQueues()[task1->getPriority()].queue.size() == 1);//the task didn't enter the wrr queue yet
//
//        s.getOrderedTaskHandler().popOrderedTask();
//        s.getOrderedTaskHandler().popOrderedTask();
//        s.getWrrQueuesScheduler().getWrrQueues()[task1->getPriority()].queue.pop();
//        s.getWrrQueuesScheduler().getWrrQueues()[task1->getPriority()].queue.pop();
//
//    }
//
//    SUBCASE("Popping tasks") {
//        auto task1 = std::make_shared<Task>(1, PrioritiesLevel::HIGHER, 10, true);
//        auto task2 = std::make_shared<Task>(2, PrioritiesLevel::HIGHER, 20, true);
//
//        s.getOrderedTaskHandler().addOrderedtask(task1);
//        s.getOrderedTaskHandler().addOrderedtask(task2);
//
//        s.getOrderedTaskHandler().popOrderedTask();
//        CHECK(s.getOrderedTaskHandler().frontOrderedTask() == task2);
//        CHECK(s.getWrrQueuesScheduler().getWrrQueues()[task1->getPriority()].queue.size() == 2);
//
//        s.getOrderedTaskHandler().popOrderedTask();
//        CHECK(s.getOrderedTaskHandler().frontOrderedTask() == nullptr);
//        CHECK(s.getWrrQueuesScheduler().getWrrQueues()[task1->getPriority()].queue.size() == 2);
//    }
//
//    SUBCASE("Empty queue behavior") {
//        CHECK(s.getOrderedTaskHandler().frontOrderedTask() == nullptr);
//
//        CHECK_NOTHROW(s.getOrderedTaskHandler().popOrderedTask());  // Should not crash
//    }
//}
=======
#include "doctest.h"
#include "../Main/Scheduler.h"
#include "Helper.h"


TEST_CASE("OrderedTaskHandler operations") {
    Scheduler s(new ReadFromJSON(), new Utility());
    clearAll(s);

    SUBCASE("Adding tasks") {
        auto task1 = std::make_shared<Task>(1,PrioritiesLevel::HIGHER, 10, true);
        auto task2 = std::make_shared<Task>(2,PrioritiesLevel::HIGHER, 20, true);

        s.getOrderedTaskHandler().addOrderedtask(task1);
        CHECK(s.getOrderedTaskHandler().frontOrderedTask() == task1);
        CHECK(s.getWrrQueuesScheduler().getWrrQueues()[task1->getPriority()].queue.size() == 1);
        CHECK(s.getWrrQueuesScheduler().getWrrQueues()[task1->getPriority()].queue.front() == task1);

        s.getOrderedTaskHandler().addOrderedtask(task2);
        CHECK(s.getOrderedTaskHandler().frontOrderedTask() == task1);
        CHECK(s.getWrrQueuesScheduler().getWrrQueues()[task1->getPriority()].queue.size() == 1);//the task didn't enter the wrr queue yet

        s.getOrderedTaskHandler().popOrderedTask();
        s.getOrderedTaskHandler().popOrderedTask();
        s.getWrrQueuesScheduler().getWrrQueues()[task1->getPriority()].queue.pop();
        s.getWrrQueuesScheduler().getWrrQueues()[task1->getPriority()].queue.pop();

    }

    SUBCASE("Popping tasks") {
        auto task1 = std::make_shared<Task>(1, PrioritiesLevel::HIGHER, 10, true);
        auto task2 = std::make_shared<Task>(2, PrioritiesLevel::HIGHER, 20, true);

        s.getOrderedTaskHandler().addOrderedtask(task1);
        s.getOrderedTaskHandler().addOrderedtask(task2);

        s.getOrderedTaskHandler().popOrderedTask();
        CHECK(s.getOrderedTaskHandler().frontOrderedTask() == task2);
        CHECK(s.getWrrQueuesScheduler().getWrrQueues()[task1->getPriority()].queue.size() == 2);

        s.getOrderedTaskHandler().popOrderedTask();
        CHECK(s.getOrderedTaskHandler().frontOrderedTask() == nullptr);
        CHECK(s.getWrrQueuesScheduler().getWrrQueues()[task1->getPriority()].queue.size() == 2);
    }

    SUBCASE("Execute Ordered Task") {
        auto task1 = std::make_shared<Task>(1, PrioritiesLevel::HIGHER, 3, true);
        s.execute(task1);
        std::this_thread::sleep_for(std::chrono::seconds(4));
        CHECK(s.getOrderedTaskHandler().frontOrderedTask() == nullptr);
    }

    SUBCASE("Insert Two Ordered Task") {
        auto task1 = std::make_shared<Task>(1, PrioritiesLevel::HIGHER, 10, true);
        auto task2 = std::make_shared<Task>(2, PrioritiesLevel::HIGHER, 20, true);
        s.insertTask(task1);
        s.insertTask(task2);

        CHECK(s.getOrderedTaskHandler().frontOrderedTask() == task1);
        s.getOrderedTaskHandler().popOrderedTask();
        CHECK(s.getOrderedTaskHandler().frontOrderedTask() == task2);
        s.getOrderedTaskHandler().popOrderedTask();
    }

    SUBCASE("Empty queue behavior") {
        CHECK(s.getOrderedTaskHandler().frontOrderedTask() == nullptr);

        CHECK_NOTHROW(s.getOrderedTaskHandler().popOrderedTask());  // Should not crash
    }


}
>>>>>>> 802cefe7a93e4484d12160e8429c67328cc213a5
