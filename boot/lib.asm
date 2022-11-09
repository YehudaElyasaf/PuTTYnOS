[bits 16]
print16:
    pusha

print16Loop:
    mov al, [bx]
    cmp al, 0
    je endPrint16Loop

    mov ah, 0eh
    int 10h
    
    add bx, 1
    jmp print16Loop

endPrint16Loop:
    popa
    ret

print16Hex:
    pusha
    cmp bl, 10
    jge print16HexAddA
    add bl, '0'
    jmp print16HexContinue
print16HexAddA:
    add bl, 'A'
print16HexContinue:
    mov al, bl
    mov ah, 0eh
    int 10h
    popa
    ret

DISK_ERROR_MSG db 'Error reading disk', 0
SECTOR_ERROR_MSG db 'Error reading sector', 0

loadKernelFromDisk:
    pusha
    push dx

    mov ah, 02h ; reading from disk
    mov al, dh  ; number of sectors to read, a parameter from caller
    
    mov ch, 0 ; cylinder
    mov cl, 2 ; sector. 1 is the boot sector, 2 is the first available sector
    mov dh, 0 ; head.
    ; dl is the drive to load from, it's a parameter from caller. 0-1 - floppies, 0x80-81 hdds. 
    ; also, es:bx is a parameter from caller, it's the address to load the data to

    int 013h
    jc diskError

    pop dx
    cmp dh, al
    jne sectorError

    popa
    ret


; receiving the data in 'dx'
; For the examples we'll assume that we're called with dx=0x1234
print_hex:
    pusha

    mov cx, 0 ; our index variable

; Strategy: get the last char of 'dx', then convert to ASCII
; Numeric ASCII values: '0' (ASCII 0x30) to '9' (0x39), so just add 0x30 to byte N.
; For alphabetic characters A-F: 'A' (ASCII 0x41) to 'F' (0x46) we'll add 0x40
; Then, move the ASCII byte to the correct position on the resulting string
hex_loop:
    cmp cx, 4 ; loop 4 times
    je end
    
    ; 1. convert last char of 'dx' to ascii
    mov ax, dx ; we will use 'ax' as our working register
    and ax, 0x000f ; 0x1234 -> 0x0004 by masking first three to zeros
    add al, 0x30 ; add 0x30 to N to convert it to ASCII "N"
    cmp al, 0x39 ; if > 9, add extra 8 to represent 'A' to 'F'
    jle step2
    add al, 7 ; 'A' is ASCII 65 instead of 58, so 65-58=7

step2:
    ; 2. get the correct position of the string to place our ASCII char
    ; bx <- base address + string length - index of char
    mov bx, HEX_OUT + 5 ; base + length
    sub bx, cx  ; our index variable
    mov [bx], al ; copy the ASCII char on 'al' to the position pointed by 'bx'
    ror dx, 4 ; 0x1234 -> 0x4123 -> 0x3412 -> 0x2341 -> 0x1234

    ; increment index and loop
    add cx, 1
    jmp hex_loop

end:
    ; prepare the parameter and call the function
    ; remember that print receives parameters in 'bx'
    mov bx, HEX_OUT
    call print16

    popa
    ret

HEX_OUT:
    db '0x0000',0 ; reserve memory for our new string


diskError:
    pusha
    mov bx, DISK_ERROR_MSG
    call print16
    
    mov al, '3'
    mov ah, 0eh
    int 10h
    popa

    ; ah stores the error code, dl stores the drive that dropped the error
    
    mov dh, ah
    call print_hex
    jmp diskLoop

sectorError:
    mov bx, SECTOR_ERROR_MSG
    call print16

diskLoop:
    jmp $