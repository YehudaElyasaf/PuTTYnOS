#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../memory/pagingHandler.h"
#include "../../lib/linkedList.h"
#include "../memory/pagingHandler.h"

#define MAX_TASK 5 //TODO: more tasks

struct Task
{
    //task ID
    uint32_t pid;

    //registers
    uint32_t esp;
    uint32_t ebp;
    uint32_t eip; //TODO: delete

    //when a task is created, 
    void(*startAddress)(void);

    bool isBlocked;
    PTEntry* pt; //TODO: delete
    struct Task* next;
};
typedef struct Task Task;


void initTasking();
uint32_t createTask(void(*startAddress)(void));
//return value: has switched
bool switchTask();
//allocate task struct //TODO: and initialize it
Task* allocateNewTask();
//get the adress of the first reserved stack
uint32_t createStack();
bool hasTaskStarted(Task* task);
