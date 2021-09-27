#include <stdio.h>
#include <pthread.h>
#include "rwlock.h"

void init_rw_lock(rwlock *lock)
{
    sem_init(&lock->lock, 0, 1);
    sem_init(&lock->writelock, 0, 1);
    lock->readers = 0;
}

void acquire_writer_lock(rwlock *lock)
{
    sem_wait(&lock->writelock);
}

void release_writer_lock(rwlock *lock)
{
    sem_post(&lock->writelock);
}

void acquire_reader_lock(rwlock *lock)
{
    sem_wait(&lock->lock);
    if (lock->readers == 0) {
        sem_wait(&lock->writelock);
    }
    lock->readers++;
    sem_post(&lock->lock);
}

void release_reader_lock(rwlock *lock)
{
    sem_wait(&lock->lock);
    lock->readers--;
    if (lock->readers == 0) {
        sem_post(&lock->writelock);
    }
    sem_post(&lock->lock);
}
