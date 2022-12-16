#include "PuTTYn.h"
#include "io/print.h"
#include "io/screen.h"
#include "memory/pagingHandler.h"
#include "cpu/idt.h"
#include "cpu/syscall.h"
#include "io/keyboard.h"
#include "../lib/string.h"
#include "../user/shell.h"
#include "../lib/syscall.h"
#include "../lib/printf.h"
#include "../lib/scanf.h"


#define _DEBUG

static inline void printDone(){
    #ifndef _DEBUG
    for(long i=0;i<50000000;i++){} //wait
    #endif
    setCursorCol(NUMBER_OF_COLS / 2);
    kcprint("Done!\n", GREEN, getBackgroundColor());
    #ifndef _DEBUG
    for(long i=0;i<20000000;i++){} //wait
    #endif
}
void initialize(){
    initScreen(GRAY, BLACK);
    
    kprint("Initializing IDT...");
    initIdt();
    printDone();

    kprint("Initializing syscalls...");
    initSyscalls();
    printDone();
    
    kprint("Initializing keyboard...");
    initKeyboard();
    printDone();

    kprint("Initializing PDT...");
    //initPDT(/*params*/);
    printDone();
    
    setColor(WHITE);
}

#ifndef _DEBUG
void main(){
    initialize();
   
    shellMain();
}
#else
#define PRINTN(x) {putchar(x/100%10+'0'); putchar(x/10%10+'0'); putchar(x%10+'0');}
void main(){
    initialize();

    //printf("%C\ta%%e%saaa\naaa%X%s%c%d%xbbb%c", CYAN, DEFAULT_COLOR, "SAVTASCHEM", 0xaa11, "\n1 - \t\t\t", '1', 234567, 0x789ABCDE, '\n');
    
    int a = 12;
    char b = ' ';
    char c[23] = {0};

    scanf("%d %c", &a, &b);

    printf("%d\n%c", a, b);

    printf("\n%d,%d\n", &a, &b);

    //printf("\n%d %c", a, b);
}
#endif
