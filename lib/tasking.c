#include "tasking.h"

void createProcess(void(*startAddress)(int, char**), int argc, char** argv){
    syscall(SYSCALL_CREATE_TASK, startAddress, argc, argv, 0);
}

bool killProcess(uint32_t pid){
    return syscall(SYSCALL_KILL_TASK, pid, 0, 0, 0);
}

void sleep(int ms){
    syscall(SYSCALL_SLEEP, ms, 0, 0, 0);
}
