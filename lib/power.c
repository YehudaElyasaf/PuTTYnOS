#include "power.h"
#include "syscall.h"

void reboot(){
    syscall(SYSCALL_REBOOT, 0, 0, 0, 0);
}

void shutdown(){
    syscall(SYSCALL_SHUTDOWN, 0, 0, 0, 0);
}