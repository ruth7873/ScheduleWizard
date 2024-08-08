# ScheduleWizard

## Authors
- Maly Fertig
- Michal Amar
- Ruth Hershler

## Project Concept Background

In today's fast-paced work environment, efficient task scheduling software has become indispensable.
These tools are crucial for organizing tasks, automating workflows, and executing routine operations with minimal human intervention. 
They significantly reduce manual effort and the potential for human error, ensuring precision and reliability in operations.

ScheduleWizard is designed to address the challenges of manual task management by offering robust automation, comprehensive integration, and reliable error handling.
Our goal is to provide a user-friendly solution that enhances productivity and operational efficiency across diverse work environments.
At its core, ScheduleWizard is a tool that schedules tasks and allocates processor time effectively among all tasks.

## Selected Algorithm

We combine a priority queue with the Weighted Round Robin (WRR) algorithm to manage and schedule tasks.
This combination ensures real-time responsiveness and fair distribution of processing time.

## Features

### Queue Tasks in Real Time
- **Real-Time Queue (RTQ):** Manages tasks requiring immediate attention, with a hijack mechanism for urgent execution.

### Priority Task Queues (Weighted Round Robin)
- **Non-Real-Time Queues:** Handles tasks with different priority levels.
- **WRR Algorithm:** Assigns tasks to queues based on their priority, with each queue having a weight and priority.
- **First-Come-First-Served (FCFS):** Manages each queue individually.

## How It Works

The scheduling algorithm ensures immediate execution of real-time tasks while maintaining fairness and efficiency for non-real-time tasks using the WRR method:

1. **Define Queues:**
   - RTQ for real-time tasks.
   - Three non-real-time queues (Queue A, Queue B, Queue C) with weights.

2. **Task Scheduling:**
   - If RTQ is not empty, execute the next real-time task, preempting non-real-time tasks.
   - If RTQ is empty, execute tasks from non-real-time queues based on WRR:
     - Run a number of tasks from each queue equal to its weight in FCFS order.

## Conclusion

ScheduleWizard seamlessly integrates advanced task scheduling features with a user-friendly interface, enhancing productivity and operational efficiency.
By effectively balancing real-time responsiveness with fair task distribution, ScheduleWizard is an essential tool for managing tasks in any fast-paced work environment.
