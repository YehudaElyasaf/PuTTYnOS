#include "syscallHandlers.h"
#include "syscall.h"
#include "../io/screen.h"
#include "../io/print.h"
#include "../io/keyboard.h"
#include "../tasking/task.h"
#include "../tasking/scheduler.h"

uint32_t notImplementedSyscallHandler(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4){
    kcprint("\nError!\n", RED, getBackgroundColor());
    kprint("Syscall ");
    kprinti(param1);
    kprint(" is not yet implemented...\n");
    
    return 0;
}

#define SYSCALL_PUTCHAR_ERROR_INVALID_COLOR 1
#define SYSCALL_PUTCHAR_ERROR_INVALID_BG_COLOR 2
uint32_t putcharSyscallHandler(uint32_t ch, uint32_t color, uint32_t backgroundColor, uint32_t param4){
    if(color > DEFAULT_COLOR)
        return SYSCALL_PUTCHAR_ERROR_INVALID_COLOR;
    if(backgroundColor > DEFAULT_COLOR)
        return SYSCALL_PUTCHAR_ERROR_INVALID_BG_COLOR;
    
    if(color == DEFAULT_COLOR)
        color = getColor();
    if(backgroundColor == DEFAULT_COLOR)
        backgroundColor = getBackgroundColor();
    
    kcprintc(ch, (uint8_t)color, (uint8_t)backgroundColor);
    
    return 0;
}

uint32_t getcharSyscallHandler(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4) {
    //TODO: block
    return kgetc();
}

uint32_t seekSyscallHandler(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4) {
    setCursorOffset(getCursorOffset() + param1);
}

uint32_t createTaskSyscallHandler(uint32_t startAddress, uint32_t param2, uint32_t param3, uint32_t param4){
    createTask(startAddress);

    return 0;
}

uint32_t killTaskSyscallHandler(uint32_t pid, uint32_t param2, uint32_t param3, uint32_t param4){
    return killTask(pid);
}

uint32_t sleepSyscallHandler(uint32_t ms, uint32_t param2, uint32_t param3, uint32_t param4){
    delayCurrentTask(ms);

    return 0;
}
