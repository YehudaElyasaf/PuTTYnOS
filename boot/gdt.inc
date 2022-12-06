gdt_start:
    dq 0 ; start with 8 null bytes

kernel_code_segment:
    dw 0xffff    ; limit = all 4 GB
    dw 0         ; part of base adress
    db 0         ; part of base adress
    ; access byte. Accessed, ReadableWriteable, Dont conform lower ring access,
    ; Code segment, not system segment, Kernel ring (2 bits), Present
    db 10011010b
    ; flags and limit (4 bits): 
    db 11001111b
    db 0         ; continue of base address

kernel_data_segment:
    dw 0xffff    ; limit = all 4 GB
    dw 0         ; part of base adress
    db 0         ; part of base adress
    ; access byte. Accessed, ReadableWriteable, Count from start up,
    ; Data segment, not system segment, User ring (2 bits), Present
    db 10010010b
    ; flags and limit (4 bits): Not long(not 64bit), Reserved, Count limit in kibibytes (go up to 4 MiB), 32bit segment
    db 11001111b
    db 0         ; continue of base address

user_code_segment:
    dw 0xffff    ; limit = all 4 GB
    dw 0         ; part of base adress
    db 0         ; part of base adress
    ; access byte. Accessed, ReadableWriteable, Dont conform lower ring access,
    ; Code segment, not system segment, Kernel ring (2 bits), Present
    db 11111010b
    ; flags and limit (4 bits): 
    db 11001111b
    db 0         ; continue of base address

user_data_segment:
    dw 0xffff    ; limit = all 4 GB
    dw 0         ; part of base adress
    db 0         ; part of base adress
    ; access byte. Accessed, ReadableWriteable, Count from start up,
    ; Data segment, not system segment, User ring (2 bits), Present
    db 11110010b
    ; flags and limit (4 bits): Not long(not 64bit), Reserved, Count limit in kibibytes (go up to 4 MiB), 32bit segment
    db 11001111b
    db 0         ; continue of base address


gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; length of gdt table (minus 1 because starting at 0)
    dd gdt_start               ; start of gdt table

CODE_SEGMENT equ kernel_code_segment - gdt_start
DATA_SEGMENT equ kernel_data_segment - gdt_start
USER_DATA_SEGMENT equ user_data_segment - gdt_start

[bits 16]
switch_to_pm:
    cli ; turn off interrupts
    lgdt [gdt_descriptor] ; load gdt descriptor
    mov eax, cr0
    or eax, 0x1 ; set 32 bit mode in cr0
    mov cr0, eax
    jmp CODE_SEGMENT:init_pm ; make the jump to handle with the pipelining


[bits 32]
switch_to_usermode:
    push ax
    mov ax, USER_DATA_SEGMENT
    mov ds, ax 
    mov es, ax 
    mov fs, ax
    mov gs, ax
    mov ss, ax
    pop ax
    ret

switch_to_kernelmode:
    push ax
    mov ax, DATA_SEGMENT
    mov ds, ax 
    mov es, ax 
    mov fs, ax
    mov gs, ax
    mov ss, ax
    pop ax
    ret

init_pm:
    call switch_to_kernelmode
    
    mov ebp, 0x90000
    mov esp, ebp

    call BEGIN_PM
    ret