#include "syscall.h"
#include "../io/print.h"

void initSyscalls(){
    irqInstallHandler(SYSCALL_IRQ, syscallIrqHandler);
}

void syscallIrqHandler(IrqFrame irqFrame){
    kprint("syscall\n");
}