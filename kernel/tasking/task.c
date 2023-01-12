#include "task.h"
#include "../asm.h"

#define DEFAULT_PAGE_DIRECTORY_SIZE 0

static uint32_t lastTaskId;

void initTasking(){
    cli();

    lastTaskId = 0;

    Task mainTaskContainer = { 0 };
    Task* mainTask = &mainTaskContainer; //TODO: = alocate task instead of using static memory
    mainTask->id = lastTaskId++;
    //TODO: init esp, ebp, eip
    mainTask->isBlocked = false;
    mainTask->pt = initPT(0); //default size
    mainTask->next = NULL;

    sti();
}
