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


diskError:
    pusha
    mov bx, DISK_ERROR_MSG
    call print16
    popa
    ; ah stores the error code, dl stores the drive that dropped the error
    
    ;mov dh, ah
    ;call print_hex
    jmp diskLoop

sectorError:
    mov bx, SECTOR_ERROR_MSG
    call print16

diskLoop:
    jmp $