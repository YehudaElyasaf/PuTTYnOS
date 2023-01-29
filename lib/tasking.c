#include "tasking.h"

void createProcess(void(*startAddress)(void)){
    syscall(SYSCALL_CREATE_TASK, startAddress, 0, 0, 0);
}

bool blockProcess(uint32_t pid){
    return syscall(SYSCALL_BLOCK_TASK, pid, 0, 0, 0);
}

bool killProcess(uint32_t pid){
    return syscall(SYSCALL_KILL_TASK, pid, 0, 0, 0);
}

void sleep(int ms){
    syscall(SYSCALL_SLEEP, ms, 0, 0, 0);
}
