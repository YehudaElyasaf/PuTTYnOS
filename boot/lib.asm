
print16:
    pusha
    mov al, [bx]
    cmp al, 0
    je endPrint16Loop

    mov ah, 0eh
    int 10h
    
    inc bx
    jmp print16

endPrint16Loop:
    popa
    ret