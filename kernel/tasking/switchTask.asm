[bits 32]
global startTask
global switchToTask

startTask:
    mov ebx, [esp + 8]; param2: startAdress
    mov esp, [esp + 4]; param1: new esp

    ;init satck
    mov ebp, esp
    
    sti
    ;run task
    jmp ebx

    hlt ;shouldn't run


switchToTask:
    mov ebp, [esp + 8] ;param2: new ebp
    mov esp, [esp + 4] ;param1: new esp

    add esp, 0x30; FIXME: why?
    popad

    sti; re-enble irqs
    iretd