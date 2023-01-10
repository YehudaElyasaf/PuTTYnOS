#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "../memory/pagingHandler.h"
#include "../../lib/linkedList.h"

typedef struct
{
    //task ID
    uint32_t id;

    //registers
    uint32_t esp;
    uint32_t ebp;
    uint32_t eip;

    bool isBlocked;
    //TODO: page directory
    struct Task* next;
} Task;

#define TASK_STRUCT_NEXT_INDEX sizeof(Task) - sizeof(Task*)

static uint32_t lastTaskId;

void initTasking();
