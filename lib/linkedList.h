#pragma once
#include "stdint.h"
#define LLEMPTY -1

void insertNode(uint32_t* head, uint32_t ptr, uint32_t nextOffset);
void removeNode(uint32_t* head, uint32_t ptr, uint32_t nextOffset);

uint32_t nodeByIndex(uint32_t head, uint32_t index, uint32_t nextOffset);
int placeInList(uint32_t head, uint32_t ptr, uint32_t nextOffset);