/*  reference for this implementation : 
*   https://locklessinc.com/articles/locks/
*/
// #include <stdio.h>
// #include <stdlib.h>

#ifndef _flagLOCK_MCS_IMPL
#define _flagLOCK_MCS_IMPL

#define cmpxchg(P, O, N) __sync_val_compare_and_swap((P), (O), (N)) //GCC PRIMITIVE

#define barrier() asm volatile("": : :"memory")
#define cpu_relax() asm volatile("pause\n": : :"memory")

static inline void *xchg_64(void *ptr, void *x)
{
    __asm__ __volatile__("xchgq %0,%1"
                :"=r" ((unsigned long long) x)
                :"m" (*(volatile long long *)ptr), "0" ((unsigned long long) x)
                :"memory");

    return x;
}
typedef struct mcs_lock_t mcs_lock_t;
struct mcs_lock_t
{
    mcs_lock_t *next;
    int flag;
};
typedef struct mcs_lock_t *mcs_lock;
static inline void lock_mcs(mcs_lock *m, mcs_lock_t *local)
{
    mcs_lock_t *tail;
    local->next = NULL;
    local->flag = 0;
    tail = xchg_64(m, local);
    //return if no tail
    if (!tail) return;
    //next is available, try to link it
    tail->next = local;
    barrier();
    while (!local->flag) cpu_relax();
    return;
}

static inline void unlock_mcs(mcs_lock *m, mcs_lock_t *local)
{
    if (!local->next)
    {
        //try unlocking
        if (cmpxchg(m, local, NULL) == local) return;
        //check for next
        while (!local->next) cpu_relax();
    }
    //Unlock next
    local->next->flag = 1; 
}

static inline int trylock_mcs(mcs_lock *m, mcs_lock_t *local)
{
    mcs_lock_t *tail;
    local->next = NULL;
    local->flag = 0;
    //try locking
    tail = cmpxchg(m, NULL, &local);
    if (!tail) return 0;
    return 1; 
}

#endif
