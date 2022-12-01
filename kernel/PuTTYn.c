#include "PuTTYn.h"
#include "io/print.h"
#include "io/screen.h"
#include "memory/pagingHandler.h"
#include "cpu/idt.h"
#include "io/keyboard.h"

void initialize(){
    initScreen(GRAY, BLACK);
    kprint("Initializing IDT...    ");
    initIdt();
    kcprint("Done!\n\n", GREEN, getBackgroundColor());

    initKeyboard();

    setColor(WHITE);
}

void main(){
    initialize();
    
    kprint("text here!!!");
    kprint("\n\nend!");
}
