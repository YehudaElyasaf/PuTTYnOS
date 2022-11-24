#include "PuTTYn.h"
#include "io/print.h"
#include "io/screen.h"
#include "memory/pagingHandler.h"
#include "idt.h"

void initialize(){
    //initPaging(0x0FF00000);
    //initIdt();
    initScreen();
}

void main(){
    initialize();
    
    //special divide by zero
    //volatile int i = 1 / 0;
    //asm __volatile__("int $3");
    //setCursorOffset(0);

    //char str1[] = "Hello\n ";
    char str1[] = "Hello";
    char* str2 = "5678";
    kcprint(str1, WHITE, LIGHT_BLUE);
    kcprint(str2, WHITE, LIGHT_BLUE);
    kcprint("42", WHITE, LIGHT_BLUE);
    setDefaultBackgroundColor(GRAY);
    clearScreen();
    kcprinti(getDefaultBackgroundColor, WHITE, LIGHT_BLUE);
}
