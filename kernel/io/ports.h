#pragma once
#include <stdint.h>

//read from port
uint8_t in8bit(uint16_t port);

//write to port
void out8bit(uint16_t port, uint8_t buffer);