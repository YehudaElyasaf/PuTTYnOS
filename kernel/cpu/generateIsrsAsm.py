ISRS_WITH_ERROR_CODE = [8, 10, 11, 12, 13, 14, 17, 21]
NUMBER_OF_ISRS = 32
GENERATED_FILE_PATH = 'kernel/cpu/isrs.asm'

file_beginning = '''; AUTO GENERATED FILE

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
'''

def has_error_code(isr_number):
    #all the following entries have an error code
    return isr_number in ISRS_WITH_ERROR_CODE

def specific_isr_genereate(isr_number):
    push_error_code_asm = ''
    if(has_error_code(isr_number)):
        push_error_code_asm = f';isr {isr_number} has error code, don\'t push dummy'
    else:
        push_error_code_asm = 'push byte 0; dummy error code'
    
    isr = f'''
global isr{isr_number}
isr{isr_number}:
    cli
    {push_error_code_asm}
    push byte {isr_number}; isr number
    jmp callIsrHandler
    '''
    return isr

with open(GENERATED_FILE_PATH, 'w') as generated_file:
    generated_file.write(file_beginning)

    for isr_number in range(NUMBER_OF_ISRS):
        generated_file.write(specific_isr_genereate(isr_number))