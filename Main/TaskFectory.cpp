//#include "TaskFectory.h"
//#include "DeadLineTask.h"
//#include "Utility.h"
//
///**
// * @brief This function allows the user to input details for a new task, including priority and running time.
// *
// * @return A new created Task object based on the user input.
// */
//Task TaskFectory::basicInput()
//{
//		std::string priority;
//		int runningTime;
//
//		std::cout << "Enter the priority for the task. Options: Critical, Higher, Middle, Lower: \n";
//		std::cin >> priority;
//
//		// Input validation for priority
//		while (priority != PrioritiesLevel::CRITICAL && priority != PrioritiesLevel::HIGHER &&
//			priority != PrioritiesLevel::MIDDLE && priority != PrioritiesLevel::LOWER) {
//			spdlog::error("Invalid priority. Please enter one of the specified options.");
//			std::cout << "Invalid priority. Please enter one of the specified options." << std::endl;
//			std::cout << "Enter the priority for the task. Options: Critical, Higher, Middle, Lower: ";
//			std::cin >> priority;
//		}
//
//		// Input validation for runningTime
//		runningTime = Utility::integerValidation();
//		spdlog::info(Logger::LoggerInfo::CREATE_NEW_TASK, priority, runningTime);
//
//		return Task((Scheduler::taskIds++) % Scheduler::MAX_TASKS, priority, runningTime);
//	}
//
//shared_ptr<Task> TaskFectory::basicTaskInput()
//{
//	Task basicTask = basicInput();
//    return std::make_shared<Task>(basicTask.getId(), basicTask.getPriority(), basicTask.getRunningTime());
//}
//
//shared_ptr<Task> TaskFectory::deadLineTaskInput()
//{
//	Task basicTask = basicInput();
//	int dealLineTime = Utility::integerValidation();
//	return std::make_shared<DeadLineTask>(basicTask, dealLineTime);
//}
//
//shared_ptr<Task> TaskFectory::repeatTaskInput()
//{
//    return shared_ptr<Task>();
//}
//
//shared_ptr<Task> TaskFectory::createTask(string type)
//{
//    if (type == "basic") {
//        return basicTaskInput();
//    }
//    else if (type == "deadLine") {
//        return deadLineTaskInput();
//    }
//    else if (type == "repeat") {
//        return repeatTaskInput();
//    }
//    else
//        return nullptr;
//}
