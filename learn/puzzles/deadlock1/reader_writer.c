#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int global_value;

sem_t wr_lock;
int reader_count;
sem_t mutex;
sem_t reader_turnstile;

void reader_lock(void)
{
    sem_wait(&mutex);
    sem_wait(&reader_turnstile);
    reader_count++;
    if (reader_count == 1) {
        sem_wait(&wr_lock);
    }
    sem_post(&reader_turnstile);
    sem_post(&mutex);
    return;
}

void reader_unlock(void)
{
    sem_wait(&mutex);
    if (reader_count == 1) {
        sem_post(&wr_lock);
    }
    reader_count--;
    sem_post(&mutex);
    return;
}

void writer_lock(void)
{
    sem_wait(&reader_turnstile);
    sem_wait(&wr_lock);
    return;
}

void writer_unlock(void)
{
    sem_post(&reader_turnstile);
    sem_post(&wr_lock);
    return;
}

void *reader_func(void *args)
{
    while (1) {
        reader_lock();

        // Reading value.
        printf("%s : Value Read : %d\n", (char *)args, global_value);

        reader_unlock();
    }
    return NULL;
}

void *writer_func(void *args)
{
    while (1) {
        writer_lock();
        global_value = rand() % 100;
        printf("%s. Written value : %d\n", (char *)args, global_value);
        writer_unlock();
    }
    return NULL;
}

int main(void)
{
    pthread_t r1, r2, r3, r4, r5;
    pthread_t w1, w2;
    sem_init(&wr_lock, 0, 1);
    sem_init(&mutex, 0, 1);
    sem_init(&reader_turnstile, 0, 1);
    global_value = 0;

    pthread_create(&w1, NULL, writer_func, "Writer1");
    pthread_create(&w2, NULL, writer_func, "Writer2");

    pthread_create(&r1, NULL, reader_func, "Reader1");
    pthread_create(&r2, NULL, reader_func, "Reader2");
    pthread_create(&r3, NULL, reader_func, "Reader3");
    pthread_create(&r4, NULL, reader_func, "Reader4");
    pthread_create(&r5, NULL, reader_func, "Reader5");

    pthread_join(w1, NULL);
    pthread_join(w2, NULL);

    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
    pthread_join(r3, NULL);
    pthread_join(r4, NULL);
    pthread_join(r5, NULL);

    sem_destroy(&wr_lock);
    return 0;
}
