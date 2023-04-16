#pragma once
#include "stdint.h"

typedef struct {
    volatile uint8_t lock;
} Spinlock;

void spin_lock(Spinlock *lock);

void spin_unlock(Spinlock *lock);
