[bits 32]
global startTask
global switchToTask

startTask:
    mov ebx, [esp + 8]; param2: startAdress
    mov esp, [esp + 4]; param1: new esp

    ;init satck
    mov esp, eax
    mov ebp, eax
    sti
    ;run task
    jmp ebx

    hlt ;shouldn't run

switchToTask:
    
    mov ebp, [esp + 8] ;param2: new esp
    mov esp, [esp + 4] ;param1: new ebp
    
    pop eax
    mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

    popa
    add esp, 4; pop isr number
    add esp, 4; pop error code
    popad
    sti; re-enble irqs
    iret
