#include "print.h"
#include "ports.h"
#include "../../lib/convert.h"

#define TAB_INDENT 4
#define MAX_LONG_ASCII_LENGTH 20 //2^64 has 20 digits

enum CHARS{
    STRING_TERMINATOR_ASCII =     '\0',
    NEWLINE_ASCII =               '\n',
    TAB_ASCII =                   '\t'
};

//private function. no declaration in print.h
void printChar(unsigned char ch, uint8_t attribute, uint16_t offset);

void kprint(char* str){
    kcprint(str, getColor(), getBackgroundColor());
}
void kprintc(char ch){
    kcprintc(ch, getColor(), getBackgroundColor());
}
void kprinti(int num){
    kcprinti(num, getColor(), getBackgroundColor());
}
void kprinth(int num){
    kcprinth(num, getColor(), getBackgroundColor());
}

void kcprint(char* str, uint8_t color, uint8_t backgroundColor){
    for(int i = 0; str[i] != STRING_TERMINATOR_ASCII; i++){
        kcprintc(str[i], color, backgroundColor);
    }
}

void kcprintc(char ch, uint8_t color, uint8_t backgroundColor){
    uint8_t attribute = (backgroundColor << 4) + color;
    printChar(ch, attribute, getCursorOffset());
}

void kcprinti(int num, uint8_t color, uint8_t backgroundColor){
    char buffer[MAX_LONG_ASCII_LENGTH] = { 0 };
    
    itoa(num, buffer);
    kcprint(buffer, color, backgroundColor);
}

void kcprinth(int num, uint8_t color, uint8_t backgroundColor){
    char buffer[MAX_LONG_ASCII_LENGTH] = { 0 };
    itoh(num, buffer);
    kcprint(buffer, color, backgroundColor);
}

//private function. no declaration in print.h
void printChar(unsigned char ch, uint8_t attribute, uint16_t offset){
    if(ch == NEWLINE_ASCII){
        //print newline
        setCursorCol(0);
        setCursorRow(getCursorRow() + 1);
    }
    else if(ch == TAB_ASCII){
        //print tab
        int numOfSpaces = (TAB_INDENT - ((getCursorCol()) % TAB_INDENT));
        for(int i = 0; i < numOfSpaces; i++)
            printChar(' ', attribute, offset + i);
    }
    else{
        uint8_t* pVideoMemory = (uint8_t*)VIDEO_MEMORY_ADDRESS;
        pVideoMemory[ATTRIBUTE_SIZE * (offset)] = ch;
        pVideoMemory[ATTRIBUTE_SIZE * (offset) + 1] = attribute;
        incCursorOffset();
    }

    if(getCursorRow() > NUMBER_OF_ROWS - 3)
        //close to end of the screen
        scrollScreen(1);
}

uint16_t getCursorOffset(){
    uint16_t offset;

    out8bit(SCREEN_CONTROL_REGISTER_ADRESS, CURSOR_OFFSET_LOW);
    uint8_t lowOffset = in8bit(SCREEN_DATA_REGISTER_ADRESS);

    out8bit(SCREEN_CONTROL_REGISTER_ADRESS, CURSOR_OFFSET_HIGH);
    uint8_t highOffset = (in8bit(SCREEN_DATA_REGISTER_ADRESS)); //half of the offset is shifted to the left

    return ((uint16_t)highOffset << (CURSOR_OFFSET_LENGTH / 2)) + lowOffset;;
}

void setCursorOffset(uint16_t offset){
    out8bit(SCREEN_CONTROL_REGISTER_ADRESS, CURSOR_OFFSET_LOW);
    out8bit(SCREEN_DATA_REGISTER_ADRESS, (uint8_t)(offset));

    out8bit(SCREEN_CONTROL_REGISTER_ADRESS, CURSOR_OFFSET_HIGH);
    out8bit(SCREEN_DATA_REGISTER_ADRESS, (uint8_t)(offset >> (CURSOR_OFFSET_LENGTH / 2)));
}

uint16_t incCursorOffset(){
    setCursorOffset(getCursorOffset() + 1);
}


uint16_t getCursorCol(){
    return getCursorOffset() % NUMBER_OF_COLS;
}

uint16_t getCursorRow(){
    return getCursorOffset() / NUMBER_OF_COLS;
}

void setCursorCol(uint16_t col){
    uint16_t row = getCursorRow();
    setCursorOffset(row * NUMBER_OF_COLS + col);
}

void setCursorRow(uint16_t row){
    uint16_t col = getCursorCol();
    setCursorOffset(row * NUMBER_OF_COLS + col);
}
