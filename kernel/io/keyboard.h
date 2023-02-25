#pragma once
#include "stdint.h"
#define KEY_BUFFER_LEN 1024

static uint8_t key_buffer[KEY_BUFFER_LEN];

void initKeyboard();

char kgetc();