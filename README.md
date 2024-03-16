**Instructions**

1. Run the "make" command, to compile and create unit test files.
2. Then, run "./run-test-script.sh" to run the code to execute, and print the metrics of each thread in the terminal.

spinlock-pthread.h - spinlock using POSIX thread mutex lock

spinlock-cmpxchg.h - spinlock test-set using cmpxchg

spinlock-xchg.h - spinlock test-set using xchg

spinlock-with-backoff.h - spinlock test-test-set using backoff

spinlock-mcs-impl.h - MCS queuelock

queuelock-clh.h - CLH queuelock

spinlock-own.h - Ticketing spinlock
