#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../memory/pagingHandler.h"
#include "../../lib/linkedList.h"
#include "../memory/pagingHandler.h"

#define MAX_TASK 100

struct Task
{
    //task ID
    uint32_t pid;

    //registers
    uint32_t esp;
    uint32_t ebp;

    //when a task is created, 
    void(*startAddress)(void);

    bool isBlocked;
    uint32_t sleepTimeMS; //to sleep syscall, in miliseconds
    PTEntry* pt; //TODO: delete?
    struct Task* next;
};
typedef struct Task Task;


void initTasking();
uint32_t createTask(void(*startAddress)(void));
//return value: has switched
bool switchTask();
//allocate task struct
Task* allocateNewTask();
//get the adress of the first reserved stack
uint32_t createStack();
bool hasTaskStarted(Task* task);
