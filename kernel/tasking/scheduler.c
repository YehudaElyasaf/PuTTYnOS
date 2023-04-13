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

Task* getTasksHead(){
    return tasksHead;
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
    
    //TODO: deallocate task
    
    //check if another task is joined to this task
    Task* mov = tasksHead;
    while(mov){
        if(mov->joinedTo == pid){
            mov->joinedTo = 0;
            mov->isBlocked = false;
            mov->sleepTimeMS = 0;
        }
        mov = mov->next;
    }
    
    task->pid = 1000;//removeTask(pid);
    task->isBlocked = true;
}

void decreaseSleepTimes(){
    Task* mov = tasksHead;

    while(mov != NULL){
        if(mov->isBlocked && mov->sleepTimeMS > 0){
            //task is sleeping
            //let's help it wake up!
            mov->sleepTimeMS--;

            if(mov->sleepTimeMS == 0)
                //Yap! We woke it!
                mov->isBlocked = false;
        }

        mov = mov->next;
    }
}

void joinTask(uint32_t pid){
    if(findTask(pid)){
        currentTask->joinedTo = pid;
        currentTask->isBlocked = true;
    }
    //call irq 0 (isr 32) - timer interrupt
    //to push regiters and switch task
    asm volatile("int $32");
}

void delayCurrentTask(uint32_t ms){
    if(currentTask == NULL)
        return;

    currentTask->isBlocked = true;
    currentTask->sleepTimeMS = ms;
}
