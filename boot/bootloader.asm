[org 07c00h]
[bits 16]
jmp start

msg db 'Hello, Putin is awaiting!', 0
STACK_START equ 0x9000
KERNEL_START equ 0x1000

start:
    mov bp, STACK_START
    mov sp, bp

    mov dh, 1
    mov dl, 80h
    mov bx, KERNEL_START
    call loadKernelFromDisk

    call switch_to_pm

[bits 32]
BEGIN_PM:
    jmp $

%include "boot/lib.asm"
%include "boot/gdt.asm"

times 510-($-$$) db 0
dw 0xaa55
