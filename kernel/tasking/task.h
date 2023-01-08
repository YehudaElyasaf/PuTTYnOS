#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../memory/pagingHandler.h"
#include "../../lib/linkedList.h"

typedef struct
{
    //task ID
    uint32_t pid;

    //registers
    uint32_t esp;
    uint32_t ebp;
    uint32_t eip;

    bool isBlocked;
    //TODO: page directory
    Task* next;
} Task;

#define TASK_STRUCT_NEXT_INDEX sizeof(Task) - sizeof(Task*)

static uint32_t lastPid;

void initTasking();
