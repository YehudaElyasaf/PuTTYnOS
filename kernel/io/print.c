#include "print.h"

#define ATTRIBUTE_SIZE 2
#define NUMBER_OF_ROWS 25
#define NUMBER_OF_COLS 84
#define VIDEO_MEMORY_ADDRESS 0xb8000

void kcprint(char* str, uint8_t color, uint8_t backgroundColor){
    for(int i = 0; str[i] == '\0'; i++){
        kcprintc(str[i], color, backgroundColor);
    }
    kcprintc('a', COLOR_RED, COLOR_BLACK);
}
void kcprintc(char ch, uint8_t color, uint8_t backgroundColor){
    uint8_t attribute = (backgroundColor << 4) + color;
    printChar(ch, attribute, getCursorOffset());
}

//private
void printChar(unsigned char ch, uint8_t attribute, int offset){
    //if(row < 0 || row > NUMBER_OF_ROWS || col < 0 || col > NUMBER_OF_COLS){
    if(0){
        //print error
    }
    else{
        uint8_t* pVideoMemory = (uint8_t*)VIDEO_MEMORY_ADDRESS;
        pVideoMemory[offset] = ch;
        pVideoMemory[offset + 1] = attribute;
    }
}

uint16_t getCursorOffset(){
    return 10;
}