#include "screen.h"
#include "print.h"

void clearScreen(){
    setCursorOffset(0);
    for(int i = 0; i < NUMBER_OF_COLS * NUMBER_OF_ROWS; i++)
        kcprintc(' ', DEFAULT_COLOR, DEFAULT_BACKGROUND_COLOR);
    
    setCursorOffset(0);
}
void scrollLines(unsigned int linesToScroll){
    for(int i = 0; i < linesToScroll; );
}