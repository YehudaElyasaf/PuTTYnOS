#include <stdint.h>

enum COLORS{
    COLOR_BLACK = 0,
    COLOR_BLUE,
    COLOR_GREEN,
    COLOR_CYAN,
    COLOR_RED,
    COLOR_PURPLE,
    COLOR_BROWN,
    COLOR_GRAY,
    COLOR_DARK_GRAY,
    COLOR_LIGHT_BLUE,
    COLOR_LIGHT_GREEN,
    COLOR_LIGHT_CYAN,
    COLOR_LIGHT_RED,
    COLOR_LIGHT_PURPLE,
    COLOR_YELLOW,
    COLOR_WHITE
};

void kcprint(char* str, uint8_t color, uint8_t backgroundColor);
void kcprintc(char ch, uint8_t color, uint8_t backgroundColor);
//void kcprinti(int num, uint8_t color, uint8_t backgroundColor);
//void kcprinth(int hexNum, uint8_t color, uint8_t backgroundColor);

uint16_t getCursorOffset(); //TODO
//void setCursorOffset(uint16_t offset);