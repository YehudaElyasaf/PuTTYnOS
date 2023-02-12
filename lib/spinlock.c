#include "spinlock.h"

void spin_lock(Spinlock *lock) {
    unsigned char prev;
    asm volatile(
        "1: xorl $0x1, %[prev]\n\t"
        "lock cmpxchgb %[prev], %[lock]\n\t"
        "jne 1b\n\t"
        : [prev] "=q" (prev), [lock] "+m" (lock->lock)
        :
        : "memory"
    );
}

void spin_unlock(Spinlock *lock) {
    asm volatile(
        "movl $0, %[lock]\n\t"
        : [lock] "+m" (lock->lock)
        :
        : "memory"
    );
}
