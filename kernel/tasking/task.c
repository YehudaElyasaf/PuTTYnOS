#include "task.h"
#include "../asm.h"
#include "../../lib/memory.h"

#define DEFAULT_PAGE_DIRECTORY_SIZE 0
#define MAGIC_NUMBER 0x5945
#define TASK_STRUCT_NEXT_INDEX sizeof(Task) - sizeof(Task*)

#define KERNEL_STACK_START  0x9000
#define STACK_SIZE_BYTES    0x2000

static uint32_t lastTaskId;
static Task* tasksHead;
static Task* currentTask;

//TODO: delete these
static Task tasksContainer[MAX_TASK + 5];

static bool reservedStacksIndexes[MAX_TASK];

void initTasking(){
    cli();

    memset(0, reservedStacksIndexes, sizeof(reservedStacksIndexes));

    currentTask = allocateNewTask();
    tasksHead = currentTask;

    //TODO: code repeating in createTask(), use function
    currentTask->id = lastTaskId++;
    currentTask->esp = createStack();
    currentTask->ebp = currentTask->esp;
    currentTask->startAdress = NULL;
    currentTask->isBlocked = false;
    currentTask->next = NULL;

    sti();
}

void createTask(uint32_t startAddres){
    cli();
    
    Task* newTask = allocateNewTask();
    newTask->id = lastTaskId++;
    newTask->esp = createStack();
    newTask->ebp = newTask->esp;
    newTask->startAdress = startAddres;
    newTask->isBlocked = false;
    newTask->next = NULL;

    //add task to end of queue
    //goto last task
    Task* mov = tasksHead;
    while(mov->next)
        mov = mov->next;
    
    mov->next = newTask;

    sti();
}

bool taskSwitch(){
    if(currentTask == NULL)
        //no multitasking
        return false;

    //save stack pointers
    asm volatile("mov %%esp, %0" : "=r"(currentTask->esp));
    asm volatile("mov %%ebp, %0" : "=r"(currentTask->ebp));
    
    /*FIXME: delete
    //save instruction pointer (eip)
    //can't be read 
    currentTask->eip = getCurrentEIP();
    if(currentTask->eip == MAGIC_NUMBER)
        //TODO: why?
        //therefore, don't switch task
        return false;
    */

   //FIXME: = nextUnblockedTask();
    Task* newTask = currentTask->next;
    if(!newTask)
        //end of tasks queue
        newTask = tasksHead;

    if(newTask->hasStarted){
        //switchToTask(newTask->esp, newTask->ebp);
    }
    else{
        //startTask(newTask->esp, newTask->startAdress);
        newTask->startAdress = NULL;
    }
}

//FIXME: use dinamic memory instead
Task* allocateNewTask(){
    return tasksContainer + lastTaskId;
}

uint32_t* createStack(){
    //FIXME: maybe a task space is needed?
    //TODO: when a task is killed, unreserve it's stack in the array
    int stackIndex = 0;
    for(stackIndex; stackIndex < MAX_TASK; stackIndex++)
        if(reservedStacksIndexes[stackIndex] == 0) //stack is unused
            break;

    return KERNEL_STACK_START + (stackIndex * STACK_SIZE_BYTES);
}

//struct Task - methods
bool hasStarted(Task* this){
    //start adress isn't NULL only if the task is waiting to start
    return this->startAdress == NULL;
}
