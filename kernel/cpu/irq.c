#include "irq.h"
#include "idt.h"
#include "../ports.h"
#include "irqs.h" //auto generated
#include "../io/print.h"

void initIrq(){
    //TODO: change order?

    //remap PIC, to avoid overriding isrs
    out8bit(PIC_MASTER_CONTROL_REGISTER, PIC_INIT_CMD);
    out8bit(PIC_SLAVE_CONTROL_REGISTER, PIC_INIT_CMD);
    out8bit(PIC_MASTER_DATA_REGISTER, FIRST_IRQ_MASTER_ENTRY_INDEX);
    out8bit(PIC_SLAVE_DATA_REGISTER, FIRST_IRQ_SLAVE_ENTRY_INDEX);
    out8bit(PIC_MASTER_DATA_REGISTER, PIC_MASTER_REMAP_CMD1);
    out8bit(PIC_SLAVE_DATA_REGISTER, PIC_SLAVE_REMAP_CMD1);
    out8bit(PIC_MASTER_DATA_REGISTER, PICS_REMAP_CMD2);
    out8bit(PIC_SLAVE_DATA_REGISTER, PICS_REMAP_CMD2);
    out8bit(PIC_MASTER_DATA_REGISTER, PICS_REMAP_CMD3);
    out8bit(PIC_SLAVE_DATA_REGISTER, PICS_REMAP_CMD3);

    for(int irqNumber = 0; irqNumber < NUM_OF_IRQS; irqNumber++) //not <=
        initIdtEntry(FIRST_IRQ_MASTER_ENTRY_INDEX + irqNumber, getIrq(irqNumber), IDT_FLAGS_INTERRUPT_GATE_RING3);
}

void irqHandler(IsrFrame isrFrame){
    kcprint("\nIRQ\n", LIGHT_RED, getBackgroundColor());
    kcprinti(isrFrame.isrNumber, LIGHT_RED, getBackgroundColor());
    hlt();
}
