#pragma once

#include <stdint.h>
#include "print.h"

void initScreen(uint8_t color, uint8_t backgroundColor);
void clearScreen();
void scrollScreen(unsigned short linesToScroll);

uint8_t getColor();
uint8_t getBackgroundColor();
void setColor(uint8_t color);
void setBackgroundColor(uint8_t color);
