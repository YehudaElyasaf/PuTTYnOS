#pragma once

#include "task.h"

#define CURRENT_TASK 0

//kmain - current process. can be empty.
void initScheduler(Task* kmain);
void insertTask(Task* newTask);
Task* getCurrentTask();
Task* getNextTask();
bool blockTask(uint32_t pid); //return value: is successful
