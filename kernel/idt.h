#pragma once

#include <stdint.h>

#define NUM_OF_IDT_ENTRIES 256

typedef struct
{
    uint16_t offsetLow; //bits 0-15
    uint16_t segmentSelector;
    uint8_t reserved; //always 0x0
    /*
    FLAGS (MSB to LSB):
        present                 1 bit   always 1
        DPL (privilege level)   2 bits
        zero bit                1 bit   always zero (beimash'lahem lama tsarich et ze?)
        gate type               4 bits  details above
    */
    uint8_t flags;
    uint16_t offsetHigh; //bits 16-31
} __attribute__((packed)) IDTEntry;

typedef struct
{
    uint16_t idtSize;
    IDTEntry* idtAdress;
} __attribute__((packed)) IDTR;


void initIdt();

/*
0x5 - task gate
0x6 - 16bit interrupt gate
0x7 - 16bit trap gate
0xe - 32bit interrupt gate
0xf - 32bit trap gate
*/