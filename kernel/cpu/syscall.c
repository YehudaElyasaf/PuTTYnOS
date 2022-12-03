#include "syscall.h"
#include "../io/print.h"

void initSyscalls(){
    //initIdtEntry(SYSCALL_IRQ, getIrq(SYSCALL_IRQ), IDT_FLAGS_INTERRUPT_GATE_RING3);
    irqInstallHandler(SYSCALL_IRQ - FIRST_IRQ_MASTER_ENTRY_INDEX, syscallIrqHandler);
}

void syscallIrqHandler(IrqFrame irqFrame){
    kprint("syscall\n");
    kprinti(irqFrame.irqNumber);
}
