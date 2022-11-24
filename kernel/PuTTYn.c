#include "PuTTYn.h"
#include "io/print.h"
#include "io/screen.h"
#include "memory/pagingHandler.h"
#include "idt.h"

void initialize(){
    //initPaging(0x0FF00000);
    //initIdt();
    clearScreen();
}

void main(){
    initialize();
    
    //special divide by zero
    //volatile int i = 1 / 0;
    //asm __volatile__("int $3");
    //setCursorOffset(0);

    char str1[] = "12\n3";
    char* str2 = "5678";
    kcprint(str1, BLACK, RED);
    kcprint(str2, BLACK, RED);
    kcprint("90", BLACK, RED);
}
