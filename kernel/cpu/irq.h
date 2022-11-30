#pragma once

#include "isr.h"

#define PIC_MASTER_CONTROL_REGISTER  0x20
#define PIC_MASTER_DATA_REGISTER     0x21
#define PIC_SLAVE_CONTROL_REGISTER   0xa0
#define PIC_SLAVE_DATA_REGISTER      0xa1

#define PIC_INIT_CMD 0x11

void initIrq();
void irqHandler(IsrFrame isrFrame);