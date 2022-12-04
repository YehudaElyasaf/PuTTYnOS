#include "syscall.h"
#include "../io/print.h"
#include "../asm.h"

static uint32_t (*syscallHandlers[NUMBER_OF_SYSCALLS])(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4) = { 0 };

void initSyscalls(){
    irqInstallHandler(SYSCALL_IDT_INDEX - FIRST_IRQ_MASTER_ENTRY_INDEX, syscallIrqHandler);

    for(int syscallIndex = 0; syscallIndex < NUMBER_OF_SYSCALLS; syscallIndex++)
        syscallHandlers[syscallIndex] = notImplementedSyscallHandler;

    //Add handlers here:
    syscallHandlers[SYSCALL_PUTCHAR]      = putcharSyscallHandler;
}

void syscallIrqHandler(IrqFrame irqFrame){
    uint32_t syscallIndex = irqFrame.regs.esi;
    uint32_t param1 =       irqFrame.regs.eax;
    uint32_t param2 =       irqFrame.regs.ebx;
    uint32_t param3 =       irqFrame.regs.ecx;
    uint32_t param4 =       irqFrame.regs.edx;

    if(syscallIndex >= NUMBER_OF_SYSCALLS){
        kcprint("Error!\n", RED, getBackgroundColor());
        kprint("\tInvalid syscall index\n");
        return;
    }
    
    uint32_t(*handler)(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4) = syscallHandlers[syscallIndex];

    if(handler == notImplementedSyscallHandler){
        //syscall nnot implemented, move syscall number in param1 (eax)
        param1 = syscallIndex;
    }

    __asm__ volatile("\
        push %1; \
        push %2; \
        push %3; \
        push %4; \
        call *%5; \
        add %6, %%esp;" //clean stack

    : "=r"(irqFrame.regs.eax)

    : "d"(param4), "c"(param3), "b"(param2), "a"(param1),
        "r"(handler), "r"(NUMBER_OF_SYSCALLS_PARAMETERS * sizeof(uint32_t))
    );
    
    //return with eax
    asm("mov %0, %%eax" : :"r"(irqFrame.regs.eax));
}
