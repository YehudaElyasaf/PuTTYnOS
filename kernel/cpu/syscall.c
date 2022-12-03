#include "syscall.h"
#include "irq.h"

void syscallIrqHandler(){

}

void initSyscalls(){
    irqInstallHandler(SYSCALL_IRQ, syscallIrqHandler);
}