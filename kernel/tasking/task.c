#include "task.h"
#include "scheduler.h"
#include "../asm.h"
#include "../../lib/linkedList.h"
#include "../../lib/memory.h"
#include "../../lib/printf.h"
#include "../../lib/heap.h"

extern void startTask(uint32_t, void(*startAddress)(int, char**), int, char**);
extern void switchTo(uint32_t, uint32_t);

#define DEFAULT_PAGE_DIRECTORY_SIZE 0
#define TASK_STRUCT_NEXT_OFFSET sizeof(Task) - sizeof(Task*)

#define KERNEL_STACK_START  0x9000
#define STACK_SIZE_BYTES    0x2000

static uint32_t lastTaskPid;

//TODO: delete this
static Task tasksContainer[MAX_TASK + 5];
static char reservedStacksIndexes[MAX_TASK];

void initTasking(){
    cli();

    memset(0, reservedStacksIndexes, sizeof(reservedStacksIndexes));
    lastTaskPid = 0;

    uint32_t esp, ebp;
    Task* kmain = allocateNewTask();

    asm volatile("mov %%esp, %0" : "=r"(esp) : );
    asm volatile("mov %%esp, %0" : "=r"(ebp) : );

    kmain->pid = ++lastTaskPid;
    kmain->esp = esp;
    kmain->ebp = ebp;
    kmain->startAddress = NULL; //task already started
    kmain->isBlocked = false;
    kmain->sleepTimeMS = 0;
    kmain->next = NULL;
    initScheduler(kmain);

    sti();
}

uint32_t createTask(void(*startAddress)(int, char**), int argc, char** argv){
    cli();
    
    Task* newTask = allocateNewTask();
    newTask->pid = ++lastTaskPid;
    newTask->esp = createStack();
    newTask->ebp = newTask->esp;
    newTask->startAddress = startAddress;
    newTask->isBlocked = false;
    newTask->sleepTimeMS = 0;
    newTask->next = NULL;
    newTask->argc = argc;
    newTask->argv = argv;
    
    insertTask(newTask);
    
    asm("\
    mov %%eax, %0   ;\
    sti             ;\
    leave           ;\
    ret             ;\
    " : : "r"(newTask->pid));
}

bool switchTask(){
    cli();
    decreaseSleepTimes();

    Task* currentTask = getCurrentTask();

    if(hasTaskStarted(currentTask)){
        //save stack pointers
        //if task hasn't started, esp and ebp are initialized to current values
        asm volatile("mov %%esp, %0" : "=r"(currentTask->esp));
        asm volatile("mov %%ebp, %0" : "=r"(currentTask->ebp));
    }
    
    Task* newTask = getNextTask();
    if(newTask == NULL){
        //shouldn't switch task
        sti();

        if(currentTask != NULL && currentTask->isBlocked){
            //Ho no! Task is blocked but we don't have any other task. We'll wait to next interrupt
            hlt();
        }

        return false;
    }

    currentTask = newTask;
    if(!hasTaskStarted(newTask)){
        //start this task

        //save start adress before we override it
        void(*startAddress)(void) = newTask->startAddress;
        newTask->startAddress = NULL;

        startTask(newTask->esp, startAddress, newTask->argc, newTask->argv);
    }
    else{
        //switch to next task
        switchTo(newTask->esp, newTask->ebp);
    }
}

Task* allocateNewTask(){
    //FIXME: use dinamic memory instead
    return tasksContainer + lastTaskPid;
}

uint32_t createStack(){
    //FIXME: maybe a task space is needed?
    //TODO: when a task is killed, unreserve it's stack in the array
    int stackIndex = 1;
    for(stackIndex; stackIndex < MAX_TASK; stackIndex++)
        if(reservedStacksIndexes[stackIndex] == false){ //stack is unused
            reservedStacksIndexes[stackIndex] = true;
            break;
        }

    return KERNEL_STACK_START + (stackIndex * STACK_SIZE_BYTES);
}

bool hasTaskStarted(Task* task){
    //start adress isn't NULL only if the task is waiting to start
    return task->startAddress == NULL;
}
