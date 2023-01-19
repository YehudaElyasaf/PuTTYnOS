#include "task.h"
#include "../asm.h"
#include "../../lib/linkedList.h"
#include "../../lib/memory.h"

#define DEFAULT_PAGE_DIRECTORY_SIZE 0
#define TASK_STRUCT_NEXT_OFFSET sizeof(Task) - sizeof(Task*)

#define KERNEL_STACK_START  0x9000
#define STACK_SIZE_BYTES    0x2000

static uint32_t lastTaskPid;
static Task* tasksHead;
static Task* currentTask;

//TODO: delete this
static Task tasksContainer[MAX_TASK + 5];

static bool reservedStacksIndexes[MAX_TASK];

void initTasking(){
    cli();

    memset(0, reservedStacksIndexes, sizeof(reservedStacksIndexes));
    lastTaskPid = 0;

    currentTask = NULL;
    tasksHead = NULL;

    sti();
}

void createTask(uint32_t* startAddres){
    cli();
    
    Task* newTask = allocateNewTask(startAddres);

    newTask->pid = ++lastTaskPid;
    newTask->esp = createStack();
    newTask->ebp = newTask->esp;
    newTask->startAdress = startAddres;
    newTask->isBlocked = false;
    newTask->next = NULL;
    
    //FIXME: use LinkedList.h
    if(!tasksHead){
        //first task
        tasksHead = newTask;
    }
    else{
        //goto last task
        Task* mov = tasksHead;
        while(mov->next)
            mov = mov->next;
        
        mov->next = newTask;
    }

    currentTask = newTask;

    sti();
}

bool switchTask(){
    cli();

    if(tasksHead == NULL)
        //no multitasking
        return false;
    if(hasTaskStarted(currentTask)){
        //save stack pointers
        asm volatile("mov %%esp, %0" : "=r"(currentTask->esp));
        asm volatile("mov %%ebp, %0" : "=r"(currentTask->ebp));
    }
    
   //FIXME: = nextUnblockedTask();
    Task* newTask = currentTask->next;
    if(!newTask)
        //end of tasks queue
        newTask = tasksHead;

    currentTask = newTask;
    if(!hasTaskStarted(newTask)){
        //start this task

        kcprint("new task!", RED, DEFAULT_COLOR);

        //save start adress before we override it
        int startAdress = newTask->startAdress;
        newTask->startAdress = NULL;

        kcprinth(newTask->esp, BROWN, DEFAULT_COLOR);
        kprintc('\n');
        startTask(newTask->esp, startAdress);
    }
    else{
        //switch to this task

        if(newTask->pid==1)
            kcprint("-", GREEN, DEFAULT_COLOR);
        else if(newTask->pid==2)
            kcprint("-", PURPLE, DEFAULT_COLOR);
        else
            kcprint("-", BROWN, DEFAULT_COLOR);

        kprinth(newTask->esp);
        kprintc('\n');
        switchToTask(newTask->esp, newTask->ebp);
    }
}

Task* allocateNewTask(uint32_t* startAddres){
    //FIXME: use dinamic memory instead
    return tasksContainer + lastTaskPid;
}

uint32_t* createStack(){
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
    return task->startAdress == NULL;
}
