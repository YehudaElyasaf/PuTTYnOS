#include "screen.h"
#include "print.h"

static uint8_t defaultColor;
static uint8_t defaultBackgroundColor;

void initScreen(uint8_t color, uint8_t backgroundColor){
    defaultColor = color;
    defaultBackgroundColor = backgroundColor;

    clearScreen();
}
uint8_t getDefaultColor(){
    return defaultColor;
}

uint8_t getDefaultBackgroundColor(){
    return defaultBackgroundColor;
}

void setDefaultColor(uint8_t color){
    defaultColor = color;
}

void setDefaultBackgroundColor(uint8_t color){
    defaultBackgroundColor = color;
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
