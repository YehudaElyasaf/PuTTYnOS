[org 07c00h]
jmp start

msg db 'Hello, Putin is awaiting!', 0

start:
    mov bx, msg
    call print16

%include "boot/lib.asm"

times 510-($-$$) db 0
dw 0xaa55
