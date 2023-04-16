[bits 32]
global startTask
global switchTo

startTask:
    mov edx, [esp + 16]; param4: argv
    mov ecx, [esp + 12]; param3: argc
    mov ebx, [esp + 8]; param2: startAdress
    mov esp, [esp + 4]; param1: new esp

    ;init satck
    mov ebp, 0xb000
    push edx; argv
    push ecx; argc
    
    sti
    ;run task
    call ebx

    hlt ;shouldn't run


switchTo:
    mov ebp, [esp + 8] ;param2: new ebp
    mov esp, [esp + 4] ;param1: new esp

    add esp, 0x30; FIXME: why?
    popad

    sti; re-enble irqs
    iretd