#include "keyboard.h"
#include <stdint.h>
#include "../cpu/irq.h"
#include "../ports.h"
#include "../../lib/string.h"
#include "../../lib/memory.h"
#include "print.h"

#define KEY_BUFFER_LEN 1024

static uint8_t key_buffer[KEY_BUFFER_LEN];
static uint16_t buffer_ptr = 0;

void pushStack(char ch) {
    int len = strlen(key_buffer + buffer_ptr);
    if (len = 0) {
        buffer_ptr = 0;
    }
    else if (buffer_ptr + len + 1 > KEY_BUFFER_LEN) {
        memcpy(key_buffer + len, key_buffer, len);
        buffer_ptr = 0;
    }
    key_buffer[buffer_ptr + len] = ch;
    key_buffer[buffer_ptr + len + 1] = 0;
}

char popStack() {
    int len = strlen(key_buffer + buffer_ptr);
    if (len = 0) {
        buffer_ptr = 0;
        return 0;
    }
    char ret = key_buffer[len + buffer_ptr];
    key_buffer[len + buffer_ptr] = 0;
    return ret;
}

char kgetc() {
    return popStack();
}

static void keyboardHandler(IsrFrame reg) {
    uint8_t scancode = in8bit(0x60);

    kprinth(scancode);
    pushStack(scancode);
}

void initKeyboard() {
    irqInstallHandler(IRQ1_KEYBOARD, keyboardHandler);
}