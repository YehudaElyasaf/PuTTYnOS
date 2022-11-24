#include "PuTTYn.h"
#include "io/print.h"
#include "io/screen.h"
#include "memory/pagingHandler.h"
#include "idt.h"

void initialize(){
    //initPaging(0x0FF00000);
    //initIdt();
    initScreen(WHITE, BLACK);
}

void main(){
    initialize();
    
    //special divide by zero
    //volatile int i = 1 / 0;
    //asm __volatile__("int $3");
    //setCursorOffset(0);

    //char str1[] = "Hello\n ";
    char str1[] = "Hello";
    kcprint(str1, RED, DARK_GRAY);
    kprint(str1);
    kcprinth(0x424242, RED, LIGHT_BLUE);
}
