#include "scheduler.h"

static Task* tasksHead;
static Task* currentTask;

void initScheduler(Task* kmain){
    currentTask = tasksHead = kmain;
}

void insertTask(Task* newTask){
    if(!tasksHead){
        //first task
        tasksHead = newTask;
        return;
    }

    //goto last task
    Task* mov = tasksHead;
    while(mov->next)
        mov = mov->next;
    
    mov->next = newTask;
}

Task* getCurrentTask(){
    return currentTask;
}

Task* getNextTask(){
    if(currentTask == NULL){
        currentTask = tasksHead;
        return currentTask;
    }

    Task* newTask = currentTask;

    do{
        newTask = newTask->next;

        if(!newTask)
            //end of tasks queue
            newTask = tasksHead;

        if(newTask == currentTask)
            return NULL;
    } while(newTask->isBlocked);
    
    currentTask = newTask;
}

static Task* findTask(uint32_t pid){
    if(pid == CURRENT_TASK)
        return currentTask;

    Task* mov = tasksHead;
    while (mov != NULL)
    {
        if(mov->pid == pid)
            return mov;

        mov = mov->next;
    }

    //couldn't find process
    return NULL;
}

bool blockTask(uint32_t pid){
    Task* task = findTask(pid);
    
    if(task == NULL)
        return false;
    
    task->isBlocked = true;
    return true;
}

bool killTask(uint32_t pid){
    Task* task = findTask(pid);
    
    if(task == NULL)
        return false;
    
    //kill task
    //TODO: deallocate task
    
}
