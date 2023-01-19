#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../memory/pagingHandler.h"
#include "../../lib/linkedList.h"
#include "../memory/pagingHandler.h"

#define MAX_TASK 5 //TODO: more tasks

typedef struct
{
    //task ID
    uint32_t pid;

    //registers
    uint32_t esp;
    uint32_t ebp;
    uint32_t eip; //TODO: delete

    //when a task is created, 
    uint32_t* startAdress;

    bool isBlocked;
    PTEntry* pt; //TODO: delete
    struct Task* next;
} Task;

void initTasking();
void createTask(uint32_t* startAddres);
//return value: has switched
bool switchTask();
//allocate task struct //TODO: and initialize it
Task* allocateNewTask(uint32_t* startAddres);
//get the adress of the first reserved stack
uint32_t* createStack();
bool hasTaskStarted(Task* task);
