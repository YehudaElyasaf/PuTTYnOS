#include "screen.h"
#include "print.h"

void initScreen(){
    defaultColor = WHITE;
    defaultBackgroundColor = RED;
    clearScreen();
}

void clearScreen(){
    setCursorOffset(0);
    for(int i = 0; i < NUMBER_OF_COLS * NUMBER_OF_ROWS; i++)
        kcprintc(' ', defaultColor, defaultBackgroundColor);
    
    setCursorOffset(0);
}
void scrollLines(unsigned int linesToScroll){
    for(int i = 0; i < linesToScroll; );
}
