#include "task.h"
#include "../asm.h"

#define DEFAULT_PAGE_DIRECTORY_SIZE 0
#define MAGIC_NUMBER 0x5945
#define TASK_STRUCT_NEXT_INDEX sizeof(Task) - sizeof(Task*)

extern uint32_t getCurrentEIP();

static uint32_t lastTaskId;
static Task* tasksHead;
static Task* currentTask;

//TODO: delete this
static Task tasksContainer[10];

void initTasking(){
    cli();

    lastTaskId = 0;

    currentTask = allocateNewTask();
    tasksHead = currentTask;

    //TODO: code repeating in createTask();
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
    
    /*TODO: delete
    //save instruction pointer (eip)
    //can't be read 
    currentTask->eip = getCurrentEIP();
    if(currentTask->eip == MAGIC_NUMBER)
        //TODO: why?
        //therefore, don't switch task
        return false;
    */

   //TODO: = nextUnblockedTask();
    Task* newTask = currentTask->next;
    if(!newTask)
        //end of tasks queue
        newTask = tasksHead;

    if(newTask->hasStarted){
        switchToTask(newTask->esp, newTask->ebp);
    }
    else{
        startTask(newTask->esp, newTask->startAdress);
        newTask->startAdress = NULL;
    }
}

//TODO: use dinamic memory instead
Task* allocateNewTask(){

}

uint32_t createStack(){

}

//methods
bool hasStarted(Task* this){
    //start adress isn't NULL only if the task is waiting to start
    return this->startAdress == NULL;
}
