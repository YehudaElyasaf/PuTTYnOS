#include "syscallHandlers.h"
#include "syscall.h"
#include "../io/screen.h"
#include "../io/print.h"
#include "../io/keyboard.h"
#include "../tasking/task.h"
#include "../tasking/scheduler.h"

#define REBOOT_PORT         0x64
#define REBOOT_CODE         0xfe
#define PREPARING_TO_REBOOT 0b10

#define BOCHS_SHUTDOWN_PORT 0xb004
#define BOCHS_SHUTDOWN_CODE 0x2000
#define QEMU_SHUTDOWN_PORT  0x604
#define QEMU_SHUTDOWN_CODE  0x2000
#define VBOX_SHUTDOWN_PORT  0x4004
#define VBOX_SHUTDOWN_CODE  0x3400

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

uint32_t rebootSyscallHandler(uint32_t ms, uint32_t param2, uint32_t param3, uint32_t param4){
    while(in8bit(REBOOT_PORT) == PREPARING_TO_REBOOT);
    out8bit(REBOOT_PORT, REBOOT_CODE);
}

uint32_t shutdownSyscallHandler(uint32_t ms, uint32_t param2, uint32_t param3, uint32_t param4){
    //FIXME: works only on VMs
    out16bit(BOCHS_SHUTDOWN_PORT, BOCHS_SHUTDOWN_CODE);
    out16bit(QEMU_SHUTDOWN_PORT, QEMU_SHUTDOWN_CODE);
    out16bit(VBOX_SHUTDOWN_PORT, VBOX_SHUTDOWN_CODE);
}
