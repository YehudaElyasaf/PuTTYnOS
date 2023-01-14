#include "irq.h"
#include "isr.h"
#include "idt.h"
#include "../asm.h"
#include "irqs.h" //auto generated
#include "../io/print.h"
#include "../timer.h"
#include "../cpu/syscall.h"

void (*irqHandlers[NUM_OF_IDT_ENTRIES- FIRST_IRQ_MASTER_ENTRY_INDEX])(IsrFrame) = {0};

void initIrq(){
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

    //install default irq handlers to all
    for(int irqNumber = IRQ0_TIMER; irqNumber < NUM_OF_IRQS; irqNumber++)
        irqInstallHandler(irqNumber, defaultIrqHandler);
    
    //TODO: do this in timer.c
    irqInstallHandler(IRQ0_TIMER, timerIrqHandler);
}

void irqHandler(IrqFrame irqFrame){
    uint32_t index = irqFrame.irqIndex;
    
    if(index >= FIRST_IRQ_MASTER_ENTRY_INDEX && index <= LAST_IRQ_MASTER_ENTRY_INDEX)
        out8bit(PIC_MASTER_CONTROL_REGISTER, PIC_EOI_CMD);
    else if(index >= FIRST_IRQ_SLAVE_ENTRY_INDEX && index <= LAST_IRQ_SLAVE_ENTRY_INDEX)
        out8bit(PIC_SLAVE_CONTROL_REGISTER, PIC_EOI_CMD);
    
    irqHandlers[index](irqFrame);
}

void irqInstallHandler(uint8_t irqNumber, void(*handler)(IsrFrame)){
    uint8_t flags = IDT_FLAGS_INTERRUPT_GATE_RING0;
    if(irqNumber == SYSCALL_IRQ_NUMBER)
        flags = IDT_FLAGS_INTERRUPT_GATE_RING3;

    initIdtEntry(FIRST_IRQ_MASTER_ENTRY_INDEX + irqNumber, getIrq(irqNumber), flags);
    irqHandlers[irqNumber + FIRST_IRQ_MASTER_ENTRY_INDEX] = handler;
}

void defaultIrqHandler(IsrFrame isrFrame){
    kcprint("\nError!\n", LIGHT_RED, getBackgroundColor());
    kprint("IRQ ");
    kprinti(isrFrame.irqNumber);
    kprint(" was called. This IRQ is not yet supported.\n");
}
