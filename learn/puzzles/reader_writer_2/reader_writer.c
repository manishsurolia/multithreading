/*
 * This Program is an extension to the last reader-writer problem. Please check
 * the last reader-writer first before proceeding further.
 *
 * If a writer arrives while there are readers in the critical section, it might
 * wait in queue forever while readers come and go. As long as a new reader
 * arrives before the last of the current readers departs, there will always be
 * at least one reader in the room.
 *
 * This situation is not a deadlock, because some threads are making progress,
 * but it is not exactly desirable. A program like this might work as long as
 * the load on the system is low, because then there are plenty of opportunities
 * for the writers. But as the load increases the behavior of the system would
 * deteriorate quickly (at least from the point of view of writers).
 */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

unsigned int buffer;

sem_t wr_mutex;
sem_t mutex;
int readers;
sem_t turnstile;

void writer_lock(void)
{
    sem_wait(&turnstile);
    sem_wait(&wr_mutex);
    return;
}

void writer_unlock(void)
{
    sem_post(&wr_mutex);
    sem_post(&turnstile);
    return;
}

void reader_lock(void)
{
    sem_wait(&turnstile);
    sem_wait(&mutex);
    if (!readers) {              // First reader takes the write lock.
        sem_wait(&wr_mutex);
    }
    readers++;
    sem_post(&mutex);
    sem_post(&turnstile);
    return;
}

void reader_unlock(void)
{
    sem_wait(&mutex);
    readers--;
    if (!readers) {
        sem_post(&wr_mutex);
    }
    sem_post(&mutex);
    return;
}

/*
 * No more than one reader is allowed at a time.
 */
void *writer_func(void *args)
{
    for (int i = 0; i < 2; i++) {
        writer_lock();
        buffer = rand() % 100;
        printf("Writer%s : Written value to buffer : %d\n", (char *)args,
                buffer);
        writer_unlock();
    }
    return NULL;
}

/*
 * There can be more than one reader allowed to read, because a reader does not
 * change any value but again, there should not be any writer writing to the
 * buffer while a reader is reading the content of buffer.
 */
void *reader_func(void *args)
{
    for (int i = 0; i < 2; i++) {
        reader_lock();
        /*
         * Here, its just a small buffer reading for this example.
         * But it can be anything like reading a link list, or buffer, or
         * anything.
         * Readers just read the value, They don't write.
         */
        printf("Reader%s : Read value from buffer : %d\n", (char *) args,
                buffer);
        reader_unlock();
    }
    return NULL;
}

int main(void)
{
    pthread_t writer1, writer2;
    pthread_t reader1, reader2, reader3, reader4, reader5, reader6, reader7;

    buffer = 0;     // Initialize the buffer value.
    readers = 0;    // Initially, there wont be any reader.

    sem_init(&wr_mutex, 0, 1);
    sem_init(&mutex, 0, 1);
    sem_init(&turnstile, 0, 1);

    pthread_create(&writer1, NULL, writer_func, "1");
    pthread_create(&writer2, NULL, writer_func, "2");

    pthread_create(&reader1, NULL, reader_func, "1");
    pthread_create(&reader2, NULL, reader_func, "2");
    pthread_create(&reader3, NULL, reader_func, "3");
    pthread_create(&reader4, NULL, reader_func, "4");
    pthread_create(&reader5, NULL, reader_func, "5");

    pthread_join(writer1, NULL);
    pthread_join(writer2, NULL);

    pthread_join(reader1, NULL);
    pthread_join(reader2, NULL);
    pthread_join(reader3, NULL);
    pthread_join(reader4, NULL);
    pthread_join(reader5, NULL);

    return 0;
}
