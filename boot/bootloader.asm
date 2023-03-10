[org 7c00h]
[bits 16]

global STACK_START

jmp _start

_start:
    mov [BOOT_DRIVE], dl
    mov bp, KERNEL_STACK_START
    mov sp, bp

    mov dh, 52 ;sectors to read
    mov dl, [BOOT_DRIVE]
    mov bx, KERNEL_START
    call loadKernelFromDisk
    call switch_to_pm
    hlt

[bits 32]
BEGIN_PM:
    call KERNEL_START
    jmp $

msg db "Hello, Putin is awaiting!", 0

KERNEL_STACK_START equ 0x9000
KERNEL_START equ 0x1000
BOOT_DRIVE db 0

%include "boot/lib.inc"
%include "boot/gdt.inc"

times 510-($-$$) db 0
dw 0xaa55
