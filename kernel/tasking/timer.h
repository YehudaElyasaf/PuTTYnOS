#pragma once

#include "../cpu/irq.h"

#define SYSTEM_FREQUNCY 1000                                //ticks per second
#define PIT_FREQUENCY   1193182                             //PIT frequency
#define PIT_DIVISOR     (PIT_FREQUENCY / SYSTEM_FREQUNCY)   //system timer period time
#define TIMER_INTERVAL  (1 / (double)SYSTEM_FREQUNCY)       //interval between ticks

void initTimer();
void timerIrqHandler(uint32_t esp, uint32_t ebp);
long getTime();
