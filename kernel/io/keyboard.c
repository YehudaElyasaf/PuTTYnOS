#include "keyboard.h"
#include <stdint.h>
#include "../cpu/irq.h"

static uint8_t key_buffer[256];

static void keyboardHandler(IsrFrame reg) {

}

void initKeyboard() {
    irqInstallHandler(IRQ1_KEYBOARD, keyboardHandler);
}