
#ifndef _QUEUELOCK_CLH_H
#define _QUEUELOCK_CLH_H

#define QUEUELOCK_ATTR static __inline __attribute__((always_inline, no_instrument_function))

#include <stdatomic.h>
#include <stdlib.h>

typedef struct qnode
{
    _Atomic int succesor_wait_flag;
}CLH_node_t;

typedef struct CLH_queuelock
{
    CLH_node_t * node;
    _Atomic (CLH_node_t *) prev;
    char padding[64]; // To prevent false sharing
}spinlock;

static CLH_node_t * CLH_create_node(int isLocked) {
    CLH_node_t * new_node = (CLH_node_t *)malloc((sizeof(CLH_node_t)));
    atomic_store_explicit(&new_node->succesor_wait_flag, isLocked, memory_order_relaxed);
    return new_node;
}

void CLH_thread_init(spinlock * curr) {
    // printf("Create thread");
    CLH_node_t * node = CLH_create_node(0);
    curr->node = node;
    atomic_store(&curr->prev, node);
}

void CLH_thread_destroy(spinlock * curr) {
    // printf("Destroy thread");
    free(atomic_load(&curr->prev));
}

QUEUELOCK_ATTR void spin_lock_clh(spinlock * lock)
{
    CLH_node_t *mynode = CLH_create_node(1);
    CLH_node_t *prev = atomic_exchange(&lock->prev, mynode);

    char prevIsLocked = atomic_load_explicit(&prev->succesor_wait_flag, memory_order_relaxed);
    while (prevIsLocked) {
        prevIsLocked = atomic_load(&prev->succesor_wait_flag);
    }
    free(prev);
    lock->node = mynode;

}

QUEUELOCK_ATTR void spin_unlock_clh(spinlock *lock)
{
    if (lock->node == NULL) {
        return;
    }
    atomic_store(&lock->node->succesor_wait_flag,0);
}

#define SPINLOCK_INITIALIZER { 0 }

#endif /* _SPINLOCK_CLH_H */
