gdt_start:
    dq 0 ; start with 8 null bytes

gdt_code_segment:
    dw 0xffff    ; limit = all 4 GB
    dw 0         ; part of base adress
    db 0         ; part of base adress
    ; access byte. Accessed, ReadableWriteable, Dont conform lower ring access,
    ; Code segment, not system segment, User ring (2 bits), Present
    db 01011111b
    ; flags and limit (4 bits): Not long(not 64bit), Reserved, Count limit in kibibytes (go up to 4 MiB), 32bit segment
    db 11001111b
    db 0         ; continue of base address

gdt_data_segment:
    dw 0xffff    ; limit = all 4 GB
    dw 0         ; part of base adress
    db 0         ; part of base adress
    ; access byte. Accessed, ReadableWriteable, Count from start up,
    ; Data segment, not system segment, User ring (2 bits), Present
    db 01001111b
    ; flags and limit (4 bits): Not long(not 64bit), Reserved, Count limit in kibibytes (go up to 4 MiB), 32bit segment
    db 11001111b
    db 0         ; continue of base address

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; length of gdt table (minus 1 because starting at 0)
    dd gdt_start               ; start of gdt table

CODE_SEGMENT equ gdt_code_segment - gdt_start
DATA_SEGMENT equ gdt_data_segment - gdt_start

switch_to_pm:
    mov ax, CODE_SEGMENT
    mov cs, ax

    mov ax, DATA_SEGMENT
    mov ds, ax 
    mov es, ax 
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    mov ebp, 0x90000
    mov esp, ebp

    call BEGIN_PM