#include "scheduler.h"

static Task* tasksHead;
static Task* currentTask;

void initScheduler(Task* kmain){
    currentTask = tasksHead = kmain;
}

void insertTask(Task* newTask){
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
}

Task* getCurrentTask(){
    return currentTask;
}

Task* getNextTask(){
    Task* newTask = NULL;

    if(currentTask == NULL){
        currentTask = tasksHead;
        return tasksHead;
    }

    newTask = currentTask->next;
    if(!newTask)
        //end of tasks queue
        newTask = tasksHead;
    
    currentTask = newTask;
}
