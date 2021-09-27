#include <stdio.h>
#include <semaphore.h>

typedef struct rwlock {
    sem_t lock; // Binary lock for just for locking/unlocking operations.
    sem_t writelock;
    int readers;
} rwlock;

void init_rw_lock(rwlock *lock);
void acquire_writer_lock(rwlock *lock);
void release_writer_lock(rwlock *lock);
void acquire_reader_lock(rwlock *lock);
void release_reader_lock(rwlock *lock);
