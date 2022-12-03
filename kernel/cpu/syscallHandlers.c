#include "syscallHandlers.h"
#include "syscall.h"

uint32_t printSyscallHandler(uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4){
    kprinti(param1);
    kprinti(param2);
    kprinti(param3);
    kprinti(param4);
    
    return 5;
}
