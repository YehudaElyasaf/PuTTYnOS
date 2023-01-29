#pragma once

#include "task.h"

#define CURRENT_TASK 0

//kmain - current process. can be empty.
void initScheduler(Task* kmain);
void insertTask(Task* newTask);
Task* getCurrentTask();
Task* getNextTask();
//return value: is successfull
bool blockTask(uint32_t pid);
//return value: is successfull
bool killTask(uint32_t pid);
//decrease sleep time to all sleeping tasks
void decreaseSleepTimes();
void delayCurrentTask(uint32_t ms);
