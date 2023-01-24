#pragma once

#include "task.h"

//kmain - current process. can be empty.
void initScheduler(Task* kmain);
void insertTask(Task* newTask);
Task* getCurrentTask();
Task* getNextTask();
