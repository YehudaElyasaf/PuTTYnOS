#pragma once

#include <stdint.h>
#include "screen.h"

enum COLORS{
    BLACK = 0,
    BLUE,
    GREEN,
    CYAN,
    RED,
    PURPLE,
    BROWN,
    GRAY,
    DARK_GRAY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_PURPLE,
    YELLOW,
    WHITE,

    DEFAULT_COLOR
};

#define VIDEO_MEMORY_ADDRESS 0xb8000

#define SCREEN_CONTROL_REGISTER 0x3d4
#define SCREEN_DATA_REGISTER    0x3d5
#define CURSOR_OFFSET_LOW       15
#define CURSOR_OFFSET_HIGH      14
#define CURSOR_OFFSET_LENGTH    16

#define NUMBER_OF_ROWS 25
#define NUMBER_OF_COLS 80
#define ATTRIBUTE_SIZE 2

//print string
void kprint(char* str);
//print char
void kprintc(char ch);
//print int
void kprinti(int num);
//print hex
void kprinth(int num);

//print colored string
void kcprint(char* str, uint8_t color, uint8_t backgroundColor);
//print colored char
void kcprintc(char ch, uint8_t color, uint8_t backgroundColor);
//print colored int
void kcprinti(int num, uint8_t color, uint8_t backgroundColor);
//print colored hex
void kcprinth(int num, uint8_t color, uint8_t backgroundColor);

uint16_t getCursorOffset();
void setCursorOffset(uint16_t offset);
uint16_t incCursorOffset();

uint16_t getCursorCol();
uint16_t getCursorRow();
void setCursorCol(uint16_t col);
void setCursorRow(uint16_t row);