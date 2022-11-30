; AUTO GENERATED FILE

KERNEL_DATA_SEGMENT equ 0x10

extern isrHandler

callIsrHandler:
    pusha
    
    ;save segment in stack, (in user mode)
    mov ax, ds
    push eax
    mov ax, KERNEL_DATA_SEGMENT ;use dx to change ds, es, fs, gs
    mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
    
    call isrHandler

    ;return to kernel segment
    pop eax
    mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

    popa
    add esp, 4; pop isr number
    add esp, 4; pop error code
    sti; re-enble interrupts
    iret

global isr0
isr0:
    cli
    push byte 0; dummy error code
    push byte 0; isr number
    jmp callIsrHandler
    
global isr1
isr1:
    cli
    push byte 0; dummy error code
    push byte 1; isr number
    jmp callIsrHandler
    
global isr2
isr2:
    cli
    push byte 0; dummy error code
    push byte 2; isr number
    jmp callIsrHandler
    
global isr3
isr3:
    cli
    push byte 0; dummy error code
    push byte 3; isr number
    jmp callIsrHandler
    
global isr4
isr4:
    cli
    push byte 0; dummy error code
    push byte 4; isr number
    jmp callIsrHandler
    
global isr5
isr5:
    cli
    push byte 0; dummy error code
    push byte 5; isr number
    jmp callIsrHandler
    
global isr6
isr6:
    cli
    push byte 0; dummy error code
    push byte 6; isr number
    jmp callIsrHandler
    
global isr7
isr7:
    cli
    push byte 0; dummy error code
    push byte 7; isr number
    jmp callIsrHandler
    
global isr8
isr8:
    cli
    ;isr 8 has error code, don't push dummy
    push byte 8; isr number
    jmp callIsrHandler
    
global isr9
isr9:
    cli
    push byte 0; dummy error code
    push byte 9; isr number
    jmp callIsrHandler
    
global isr10
isr10:
    cli
    ;isr 10 has error code, don't push dummy
    push byte 10; isr number
    jmp callIsrHandler
    
global isr11
isr11:
    cli
    ;isr 11 has error code, don't push dummy
    push byte 11; isr number
    jmp callIsrHandler
    
global isr12
isr12:
    cli
    ;isr 12 has error code, don't push dummy
    push byte 12; isr number
    jmp callIsrHandler
    
global isr13
isr13:
    cli
    ;isr 13 has error code, don't push dummy
    push byte 13; isr number
    jmp callIsrHandler
    
global isr14
isr14:
    cli
    ;isr 14 has error code, don't push dummy
    push byte 14; isr number
    jmp callIsrHandler
    
global isr15
isr15:
    cli
    push byte 0; dummy error code
    push byte 15; isr number
    jmp callIsrHandler
    
global isr16
isr16:
    cli
    push byte 0; dummy error code
    push byte 16; isr number
    jmp callIsrHandler
    
global isr17
isr17:
    cli
    ;isr 17 has error code, don't push dummy
    push byte 17; isr number
    jmp callIsrHandler
    
global isr18
isr18:
    cli
    push byte 0; dummy error code
    push byte 18; isr number
    jmp callIsrHandler
    
global isr19
isr19:
    cli
    push byte 0; dummy error code
    push byte 19; isr number
    jmp callIsrHandler
    
global isr20
isr20:
    cli
    push byte 0; dummy error code
    push byte 20; isr number
    jmp callIsrHandler
    
global isr21
isr21:
    cli
    ;isr 21 has error code, don't push dummy
    push byte 21; isr number
    jmp callIsrHandler
    
global isr22
isr22:
    cli
    push byte 0; dummy error code
    push byte 22; isr number
    jmp callIsrHandler
    
global isr23
isr23:
    cli
    push byte 0; dummy error code
    push byte 23; isr number
    jmp callIsrHandler
    
global isr24
isr24:
    cli
    push byte 0; dummy error code
    push byte 24; isr number
    jmp callIsrHandler
    
global isr25
isr25:
    cli
    push byte 0; dummy error code
    push byte 25; isr number
    jmp callIsrHandler
    
global isr26
isr26:
    cli
    push byte 0; dummy error code
    push byte 26; isr number
    jmp callIsrHandler
    
global isr27
isr27:
    cli
    push byte 0; dummy error code
    push byte 27; isr number
    jmp callIsrHandler
    
global isr28
isr28:
    cli
    push byte 0; dummy error code
    push byte 28; isr number
    jmp callIsrHandler
    
global isr29
isr29:
    cli
    push byte 0; dummy error code
    push byte 29; isr number
    jmp callIsrHandler
    
global isr30
isr30:
    cli
    push byte 0; dummy error code
    push byte 30; isr number
    jmp callIsrHandler
    
global isr31
isr31:
    cli
    push byte 0; dummy error code
    push byte 31; isr number
    jmp callIsrHandler
    