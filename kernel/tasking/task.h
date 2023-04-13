#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../memory/pagingHandler.h"
#include "../../lib/linkedList.h"
#include "../memory/pagingHandler.h"

#define MAX_TASK 100

typedef struct
{
    //task ID
    uint32_t pid;

    //registers
    uint32_t esp;
    uint32_t ebp;

    //when a task is created, 
    void(*startAddress)(void);
    uint32_t argc;
    uint32_t argv;

    bool isBlocked;
    uint32_t sleepTimeMS;   //to sleep syscall, in miliseconds
    uint32_t joinedTo;      //which task this task is waiting for (by PID). if none - value us 0.
    PTEntry* pt; //TODO: delete?
    struct Task* next;
} Task;

void initTasking();
uint32_t createTask(void(*startAddress)(int, char**), int argc, char** argv);
//return value: has switched
bool switchTask();
//allocate task struct
Task* allocateNewTask();
//get the adress of the first reserved stack
uint32_t createStack();
bool hasTaskStarted(Task* task);
