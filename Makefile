CFLAGS = -O2 -g -std=gnu99 -Wall
LDFLAGS = -lpthread

programs = test-spinlock-xchg test-spinlock-pthread test-spinlock-cmpxchg test-spinlock-with-backoff test-spinlock-mcs-impl test-queuelock-clh test-spinlock-own

all: $(programs)

test-spinlock-xchg: test-spinlock.c
	$(CC) $(CFLAGS) -DXCHG $^ -o $@ $(LDFLAGS)

test-spinlock-pthread: test-spinlock.c
	$(CC) $(CFLAGS) -DPTHREAD $^ -o $@ $(LDFLAGS)

test-spinlock-cmpxchg: test-spinlock.c
	$(CC) $(CFLAGS) -DCMPXCHG $^ -o $@ $(LDFLAGS)

test-spinlock-with-backoff: test-spinlock.c
	$(CC) $(CFLAGS) -DXCHGBACKOFF $^ -o $@ $(LDFLAGS)

test-spinlock-mcs-impl: test-spinlock.c
	$(CC) $(CFLAGS) -DMCS $^ -o $@ $(LDFLAGS)

test-queuelock-clh: test-spinlock.c
	$(CC) $(CFLAGS) -DCLH $^ -o $@ $(LDFLAGS)

test-spinlock-own: test-spinlock.c
	$(CC) $(CFLAGS) -DOWN $^ -o $@ $(LDFLAGS)

%:%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -f *.o
	-rm -f $(programs)
