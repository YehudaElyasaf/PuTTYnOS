[org 7c00h]
[bits 16]
jmp start

start:
    mov [BOOT_DRIVE], dl
    mov bp, STACK_START
    mov sp, bp

    mov dh, 2
    mov dl, [BOOT_DRIVE]
    mov bx, KERNEL_START
    call loadKernelFromDisk
    call switch_to_pm
    jmp $

[bits 32]
BEGIN_PM:
    call KERNEL_START
    jmp $

msg db "Hello, Putin is awaiting!", 0
STACK_START equ 0x9000
KERNEL_START equ 0x1000
BOOT_DRIVE db 0

%include "boot/lib.asm"
%include "boot/gdt.asm"

times 510-($-$$) db 0
dw 0xaa55
