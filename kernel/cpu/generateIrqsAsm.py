NUMBER_OF_IRQS = 16
GENERATED_FILE_PATH = 'kernel/cpu/irqs.asm'

file_beginning = '''; AUTO GENERATED FILE

KERNEL_DATA_SEGMENT equ 0x10

extern irqHandler

callIrqHandler:
    pusha
    
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
    pop eax
    mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

    popa
    add esp, 4; pop irq number
    add esp, 4; pop error code
    sti; re-enble irqs
    iret
'''

def specific_irq_genereate(irq_number):    
    push_error_code_asm = ''
    
    return f'''
global irq{irq_number}
irq{irq_number}:
    cli
    {push_error_code_asm}
    push byte {irq_number}; irq number
    jmp callIrqHandler
'''

with open(GENERATED_FILE_PATH, 'w') as generated_file:
    generated_file.write(file_beginning)

    for irq_number in range(NUMBER_OF_IRQS):
        generated_file.write(specific_irq_genereate(irq_number))
