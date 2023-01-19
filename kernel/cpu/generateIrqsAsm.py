NUMBER_OF_IRQS = 16
FIRST_IRQ_INDEX = 32
SYSCALL_IRQ_NUMBER = 0x42 - FIRST_IRQ_INDEX

GENERATED_FILE_PATH = 'kernel/cpu/irqs.asm'

FILE_BEGINNING = '''; AUTO GENERATED FILE

KERNEL_DATA_SEGMENT equ 0x10

extern irqHandler

callIrqHandler:
    ;my pusha
    push eax
    push ecx
    push edx
    push ebx
    push esp
    push ebp
    push esi
    push edi
    
    ;save segment in stack, (in user mode)
    mov ax, ds
    push eax
    mov ax, KERNEL_DATA_SEGMENT ;use dx to change ds, es, fs, gs
    mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
    
    call irqHandler

    ;return to kernel segment
    pop ebx
    mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx

    ;mov eax, 5

    ;popa except eax
    pop edi
    pop esi
    pop ebp
    pop esp
    pop ebx
    pop edx
    pop ecx
    add esp, 4; instead of pop eax, because syscalls returns with eax
    
    add esp, 4; pop irq number
    add esp, 4; pop irq index in IDT
    sti; re-enable irqs
    iret
'''
TIMER_IRQ_HANDLER_CODE = '''
global irq0
extern timerIrqHandler
irq0:
    cli
    
    pushad

    push ebp
    push esp
    call timerIrqHandler
    pop eax
    pop eax

    popad

    sti
    iret
'''

def specific_irq_genereate(irq_number):
    return f'''
global irq{irq_number}
irq{irq_number}:
    cli
    push byte {irq_number}; irq number
    push byte {irq_number + FIRST_IRQ_INDEX}; irq index int IDT
    jmp callIrqHandler
'''

with open(GENERATED_FILE_PATH, 'w') as generated_file:
    generated_file.write(FILE_BEGINNING)
    generated_file.write(TIMER_IRQ_HANDLER_CODE)

    for irq_number in range(1, NUMBER_OF_IRQS):
        generated_file.write(specific_irq_genereate(irq_number))
    
    generated_file.write('\n\n;syscall irq')
    generated_file.write(specific_irq_genereate(SYSCALL_IRQ_NUMBER))
