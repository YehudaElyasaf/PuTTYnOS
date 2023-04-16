#include "tasking.h"
#include "heap.h"

uint32_t createProcess(int(*startAddress)(int, char**), int argc, char** argv, char* name){
    return syscall(SYSCALL_CREATE_TASK, startAddress, argc, argv, name);
}

void exit(int returnValue){
    syscall(SYSCALL_EXIT, returnValue, 0, 0, 0);
}

bool kill(uint32_t pid){
    return syscall(SYSCALL_KILL_TASK, pid, 0, 0, 0);
}

void sleep(int ms){
    syscall(SYSCALL_SLEEP, ms, 0, 0, 0);
}

void join(uint32_t pid){
    syscall(SYSCALL_JOIN_TASK, pid, 0, 0, 0);
}
