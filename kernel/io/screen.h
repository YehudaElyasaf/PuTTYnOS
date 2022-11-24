#pragma once

#include <stdint.h>

void initScreen(uint8_t color, uint8_t backgroundColor);
void clearScreen();
void scrollLines(unsigned int linesToScroll); //TODO

uint8_t getDefaultColor();
uint8_t getDefaultBackgroundColor();
void setDefaultColor(uint8_t color);
void setDefaultBackgroundColor(uint8_t color);