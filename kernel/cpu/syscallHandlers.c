#include "syscallHandlers.h"
#include "syscall.h"
#include "../io/screen.h"

uint32_t notImplementedSyscallHandler(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4){
    kcprint("\nError!\n", RED, getBackgroundColor());
    kprint("Syscall ");
    kprinti(param1);
    kprint(" is not yet implemented...\n");
    
    return 0;
}
uint32_t printSyscallHandler(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4){
    kprinti(param1);
    kprinti(param2);
    kprinti(param3);
    kprinti(param4);
    
    return 5;
}
