#ifndef _SPINLOCK_OWN_H
#define _SPINLOCK_OWN_H

#define barrier() asm volatile("": : :"memory")
#define cpu_relax() asm volatile("pause\n": : :"memory")

#include <stdatomic.h>
#include <stdlib.h>

#define SPINLOCK_ATTR static __inline __attribute__((always_inline, no_instrument_function))

typedef struct ticketlock {
    _Atomic unsigned short ticket;
    _Atomic unsigned short owner;
}spinlock;

SPINLOCK_ATTR void spin_lock(spinlock * lock) {
    unsigned short curr = atomic_fetch_add(&lock->owner, 1);
    while (lock->ticket != curr) cpu_relax();
}

SPINLOCK_ATTR void spin_unlock(spinlock * lock) {
    barrier();
    lock->ticket++;
}

#endif