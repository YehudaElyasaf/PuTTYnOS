#include "timer.h"
#include "../io/print.h"
#include "../asm.h"
#include "../tasking/task.h"

#define PIT_TIMER_CONTROL_REGISTER  0x43
#define PIT_TIMER_INIT_BYTE         0x36
#define PIT_TIMER_DATA_REGISTER     0x40

#define MAX_UINT16_VALUE    0xFFFF

static long ticks;

void initTimer(){
    //reset static variables
    ticks = 0;
    
    if(SYSTEM_FREQUNCY > MAX_UINT16_VALUE){
        //too high frequency, can't be written to the timer data register
        kcprint("\nInvalid system frequency (", RED, DEFAULT_COLOR);
        kcprinti(SYSTEM_FREQUNCY, RED, DEFAULT_COLOR);
        kcprint("Hz)\n", RED, DEFAULT_COLOR);
        kcprint("Initialization failed.", RED, DEFAULT_COLOR);
        cli();
        hlt();
    }

    uint8_t divisorLow  = (uint8_t) PIT_DIVISOR;
    uint8_t divisorHigh = (uint8_t) (PIT_DIVISOR >> 8);

    //out8bit(PIT_TIMER_CONTROL_REGISTER, PIT_TIMER_INIT_BYTE);
    //set the frequncy register
    out8bit(PIT_TIMER_DATA_REGISTER, divisorLow);
    out8bit(PIT_TIMER_DATA_REGISTER, divisorHigh);

    irqInstallHandler(IRQ0_TIMER, timerIrqHandler);
}

void timerIrqHandler(){
    //inform that interrupt was handled
    out8bit(PIC_MASTER_CONTROL_REGISTER, PIC_EOI_CMD);
    
    ticks++;
    
    switchTask();
}


long getTime(){
    return ticks * TIMER_INTERVAL;
}
