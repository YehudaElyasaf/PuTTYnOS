#include "keyboard.h"
#include <stdint.h>
#include "../cpu/irq.h"
#include "../asm.h"
#include "../../lib/string.h"
#include "../../lib/memory.h"
#include "print.h"

#define KEY_BUFFER_LEN 1024

static uint8_t key_buffer[KEY_BUFFER_LEN];
static uint16_t buffer_ptr = 0;

void pushQueue(char ch) {
    int len = strlen(key_buffer + buffer_ptr);
    if (len >= KEY_BUFFER_LEN) return;
    if (len == 0) {
        buffer_ptr = 0;
    }
    else if (buffer_ptr + len + 1 > KEY_BUFFER_LEN) {
        memcpy(key_buffer + buffer_ptr, key_buffer, len);
        buffer_ptr = 0;
    }

    key_buffer[buffer_ptr + len] = ch;
    key_buffer[buffer_ptr + len+1] = 0;
}

char popQueue() {
    char ret = key_buffer[buffer_ptr];
    key_buffer[buffer_ptr] = 0;
    buffer_ptr = buffer_ptr < KEY_BUFFER_LEN ? buffer_ptr + 1 : 0;
    return ret;
}

char kgetc() {
    char tmp = popQueue();
    kprinth(tmp);
    return tmp;
}

static void keyboardIrqHandler(IrqFrame reg) {
    uint8_t scancode = in8bit(0x60);

    if (scancode >= 0x80) return;

    kprinth(scancode);
    kprintc('\n');
    pushQueue(scancode);
}

void initKeyboard() {
    irqInstallHandler(IRQ1_KEYBOARD, keyboardIrqHandler);
    in8bit(0x60); //clean port 0x60
}
