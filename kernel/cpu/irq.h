#pragma once

#include "isr.h"

#define PIC_MASTER_CONTROL_REGISTER  0x20
#define PIC_MASTER_DATA_REGISTER     0x21
#define PIC_SLAVE_CONTROL_REGISTER   0xa0
#define PIC_SLAVE_DATA_REGISTER      0xa1

#define PIC_INIT_CMD    0x11
#define PIC_EOI_CMD     0x20

enum IRQS{
    IRQ0_TIMER,
    IRQ1_KEYBOARD,
    IRQ2_PIC2,
    IRQ3_COM2,
    IRQ4_COM1,
    IRQ5_LPT2,
    IRQ6_FLOPPY_DISK,
    IRQ7_LPT1,
    IRQ8_REAL_TIME_CLOCK,
    IRQ9_GENERAL_IO,
    IRQ10_GENERAL_IO,
    IRQ11_GENERAL_IO,
    IRQ12_GENERAL_IO,
    IRQ13_COMPRESSOR,
    IRQ14_IDE_BUS,
    IRQ15_IDE_BUS
};

void initIrq();
void irqHandler(IsrFrame isrFrame);
void irqInstallHandler(uint8_t irqNumber, void(*handler)(IsrFrame));
void defaultIrqHandler(IsrFrame isrFrame);
