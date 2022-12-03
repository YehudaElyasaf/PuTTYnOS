#include "irq.h"
#include "isr.h"
#include "idt.h"
#include "../asm.h"
#include "irqs.h" //auto generated
#include "../io/print.h"
#include "../timer.h"

void (*irqHandlers[NUM_OF_IDT_ENTRIES])(IsrFrame) = {0};

void initIrq(){
    //TODO: change order?

    //remap PIC, to avoid overriding isrs
    //remap PIC 1
    out8bit(PIC_MASTER_CONTROL_REGISTER, PIC_INIT_CMD);
    out8bit(PIC_MASTER_DATA_REGISTER, FIRST_IRQ_MASTER_ENTRY_INDEX);
    out8bit(PIC_MASTER_DATA_REGISTER, PIC_MASTER_REMAP_CMD1);
    out8bit(PIC_MASTER_DATA_REGISTER, PICS_REMAP_CMD2);
    out8bit(PIC_MASTER_DATA_REGISTER, PICS_REMAP_CMD3);
    
    //remap PIC 2
    out8bit(PIC_SLAVE_DATA_REGISTER, FIRST_IRQ_SLAVE_ENTRY_INDEX);
    out8bit(PIC_SLAVE_DATA_REGISTER, PIC_SLAVE_REMAP_CMD1);
    out8bit(PIC_SLAVE_DATA_REGISTER, PICS_REMAP_CMD2);
    out8bit(PIC_SLAVE_CONTROL_REGISTER, PIC_INIT_CMD);
    out8bit(PIC_SLAVE_DATA_REGISTER, PICS_REMAP_CMD3);

    for(int irqNumber = 0; irqNumber < NUM_OF_IRQS; irqNumber++) //not <=
        initIdtEntry(FIRST_IRQ_MASTER_ENTRY_INDEX + irqNumber, getIrq(irqNumber), IDT_FLAGS_INTERRUPT_GATE_RING3);

    //install default irq handlers to all
    for(int irqNumber = IRQ0_TIMER; irqNumber <= IRQ15_IDE_BUS; irqNumber++)
        irqInstallHandler(irqNumber, defaultIrqHandler);    
    
    //TODO: di this in timer.c
    irqInstallHandler(IRQ0_TIMER, timerIrqHandler);
}

void irqHandler(IsrFrame isrFrame){
    if(isrFrame.irqIndex >= FIRST_IRQ_SLAVE_ENTRY_INDEX)
        out8bit(PIC_SLAVE_CONTROL_REGISTER, PIC_EOI_CMD);
    out8bit(PIC_MASTER_CONTROL_REGISTER, PIC_EOI_CMD);

    if(irqHandlers[isrFrame.irqIndex])
        irqHandlers[isrFrame.irqIndex](isrFrame);
}

void irqInstallHandler(uint8_t irqNumber, void(*adress)(IsrFrame)){
    irqHandlers[irqNumber + FIRST_IRQ_MASTER_ENTRY_INDEX] = adress;
}

void defaultIrqHandler(IsrFrame isrFrame){
    kcprint("\nError!\n", LIGHT_RED, getBackgroundColor());
    kprint("IRQ ");
    kprinti(isrFrame.irqNumber);
    kprint(" was called. This IRQ is not yet supported.\n");
}
