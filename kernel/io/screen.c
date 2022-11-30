#include "screen.h"
#include "../../lib/memory.h"

static uint8_t defaultColor;
static uint8_t defaultBackgroundColor;

void initScreen(uint8_t color, uint8_t backgroundColor){
    defaultColor = color;
    defaultBackgroundColor = backgroundColor;

    clearScreen();
}
uint8_t getColor(){
    return defaultColor;
}

uint8_t getBackgroundColor(){
    return defaultBackgroundColor;
}

void setColor(uint8_t color){
    defaultColor = color;
}

void setdColor(uint8_t color){
    defaultBackgroundColor = color;
}


void clearScreen(){
    setCursorOffset(0);
    for(int i = 0; i < NUMBER_OF_COLS * NUMBER_OF_ROWS; i++)
        kcprintc(' ', defaultColor, defaultBackgroundColor);
    
    setCursorOffset(0);
}
void scrollScreen(unsigned short linesToScroll){
    for(int i = 0; i < linesToScroll; i++){ //start from 1, copy line 1 to 0, 2 to 1, etc.
        //scroll line by line
        for (int row = 1; row < NUMBER_OF_ROWS; row++){
            uint8_t* src = (uint8_t*)VIDEO_MEMORY_ADDRESS + (row * NUMBER_OF_COLS * ATTRIBUTE_SIZE);
            uint8_t* dst = (uint8_t*)VIDEO_MEMORY_ADDRESS + ((row - 1) * NUMBER_OF_COLS * ATTRIBUTE_SIZE);
            memcpy(src, dst, NUMBER_OF_COLS * ATTRIBUTE_SIZE);
        }
        setCursorRow(getCursorRow() - 1); //scroll cursor offset too
    }
}
