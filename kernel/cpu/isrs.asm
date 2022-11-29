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

isr0:
    cli
    push byte 0; dummy error code
    push byte 0; isr number
    jmp callIsrHandler
    
isr1:
    cli
    push byte 0; dummy error code
    push byte 1; isr number
    jmp callIsrHandler
    
isr2:
    cli
    push byte 0; dummy error code
    push byte 2; isr number
    jmp callIsrHandler
    
isr3:
    cli
    push byte 0; dummy error code
    push byte 3; isr number
    jmp callIsrHandler
    
isr4:
    cli
    push byte 0; dummy error code
    push byte 4; isr number
    jmp callIsrHandler
    
isr5:
    cli
    push byte 0; dummy error code
    push byte 5; isr number
    jmp callIsrHandler
    
isr6:
    cli
    push byte 0; dummy error code
    push byte 6; isr number
    jmp callIsrHandler
    
isr7:
    cli
    push byte 0; dummy error code
    push byte 7; isr number
    jmp callIsrHandler
    
isr8:
    cli
    ;isr 8 has error code, don't push dummy
    push byte 8; isr number
    jmp callIsrHandler
    
isr9:
    cli
    push byte 0; dummy error code
    push byte 9; isr number
    jmp callIsrHandler
    
isr10:
    cli
    ;isr 10 has error code, don't push dummy
    push byte 10; isr number
    jmp callIsrHandler
    
isr11:
    cli
    ;isr 11 has error code, don't push dummy
    push byte 11; isr number
    jmp callIsrHandler
    
isr12:
    cli
    ;isr 12 has error code, don't push dummy
    push byte 12; isr number
    jmp callIsrHandler
    
isr13:
    cli
    ;isr 13 has error code, don't push dummy
    push byte 13; isr number
    jmp callIsrHandler
    
isr14:
    cli
    ;isr 14 has error code, don't push dummy
    push byte 14; isr number
    jmp callIsrHandler
    
isr15:
    cli
    push byte 0; dummy error code
    push byte 15; isr number
    jmp callIsrHandler
    
isr16:
    cli
    push byte 0; dummy error code
    push byte 16; isr number
    jmp callIsrHandler
    
isr17:
    cli
    ;isr 17 has error code, don't push dummy
    push byte 17; isr number
    jmp callIsrHandler
    
isr18:
    cli
    push byte 0; dummy error code
    push byte 18; isr number
    jmp callIsrHandler
    
isr19:
    cli
    push byte 0; dummy error code
    push byte 19; isr number
    jmp callIsrHandler
    
isr20:
    cli
    push byte 0; dummy error code
    push byte 20; isr number
    jmp callIsrHandler
    
isr21:
    cli
    ;isr 21 has error code, don't push dummy
    push byte 21; isr number
    jmp callIsrHandler
    
isr22:
    cli
    push byte 0; dummy error code
    push byte 22; isr number
    jmp callIsrHandler
    
isr23:
    cli
    push byte 0; dummy error code
    push byte 23; isr number
    jmp callIsrHandler
    
isr24:
    cli
    push byte 0; dummy error code
    push byte 24; isr number
    jmp callIsrHandler
    
isr25:
    cli
    push byte 0; dummy error code
    push byte 25; isr number
    jmp callIsrHandler
    
isr26:
    cli
    push byte 0; dummy error code
    push byte 26; isr number
    jmp callIsrHandler
    
isr27:
    cli
    push byte 0; dummy error code
    push byte 27; isr number
    jmp callIsrHandler
    
isr28:
    cli
    push byte 0; dummy error code
    push byte 28; isr number
    jmp callIsrHandler
    
isr29:
    cli
    push byte 0; dummy error code
    push byte 29; isr number
    jmp callIsrHandler
    
isr30:
    cli
    push byte 0; dummy error code
    push byte 30; isr number
    jmp callIsrHandler
    
isr31:
    cli
    push byte 0; dummy error code
    push byte 31; isr number
    jmp callIsrHandler
    
;write isrs in array
global isrTable
isrTable:
	dd isr0
	dd isr1
	dd isr2
	dd isr3
	dd isr4
	dd isr5
	dd isr6
	dd isr7
	dd isr8
	dd isr9
	dd isr10
	dd isr11
	dd isr12
	dd isr13
	dd isr14
	dd isr15
	dd isr16
	dd isr17
	dd isr18
	dd isr19
	dd isr20
	dd isr21
	dd isr22
	dd isr23
	dd isr24
	dd isr25
	dd isr26
	dd isr27
	dd isr28
	dd isr29
	dd isr30
	dd isr31
