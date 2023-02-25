#include "keyboard.h"
#include <stdint.h>
#include "../cpu/irq.h"
#include "../asm.h"
#include "../../lib/string.h"
#include "../../lib/memory.h"
#include "print.h"
#include "../../lib/queue.h"

#define LSHIFT_SC 0x2a + 0x80
#define RSHIFT_SC 0x36 + 0x80

Queue keyQueue;

char kgetc() {
    char tmp = 0;
    queuePop(&keyQueue, &tmp);
    return tmp;
}

static void keyboardIrqHandler(IrqFrame reg) {
    uint8_t scancode = in8bit(0x60);
    in8bit(0x60); //clean port 0x60

    if (scancode >= 0x80) {
        if (scancode != LSHIFT_SC && scancode != RSHIFT_SC)
            return;
    }

    queuePush(&keyQueue, &scancode);
}

void initKeyboard() {
    // for some reason if makes sti() to make the system crash
    keyQueue = (Queue){key_buffer, 0, KEY_BUFFER_LEN, 1};

    irqInstallHandler(IRQ1_KEYBOARD, keyboardIrqHandler);
    in8bit(0x60); //clean port 0x60
}
