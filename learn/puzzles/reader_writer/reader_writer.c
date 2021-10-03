/*
 * This Program is to show how to synchronize the reading and writing by
 * multiple threads. Some threads, write the buffer, some read the buffer.
 * Readers never change any value of buffer. Hence more than one reader is
 * allowed to read the buffer.
 *
 * Writers write to the buffer, Hence writer need to take lock. Also the first
 * reader also need to take writer lock, so that writer can not take it and
 * readers leave it when all the readers are done.
 *
 * With this program we are able to synchronize the writers and readers, but
 * now we face the issue of starvation for writer.
 * Suppose there is only one writer(writing infinitely) and two
 * readers(reading infinitely).
 *
 * Once readers start reading, they don't allow the writer thread to write
 * because there is no read lock, so readers simply come and read. Only a global
 * lock is used, so there is very less contention.
 *
 * check the code below understand the logic.
 */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

unsigned int buffer;

sem_t writer_lock;
sem_t mutex;
int readers;

/*
 * No more than one reader is allowed at a time.
 */
void *writer_func(void *args)
{
    while(1) {
        sem_wait(&writer_lock);
        buffer = rand() % 100;
        printf("Writer : Written value to buffer : %d\n", buffer);
        sleep(1);
        sem_post(&writer_lock);
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
    while (1) {
        sem_wait(&mutex);
        if (!readers) {              // First reader takes the write lock.
            sem_wait(&writer_lock);
        }
        readers++;
        sem_post(&mutex);

        /*
         * Here, its just a small buffer reading for this example.
         * But it can be anything like reading a link list, or buffer, or anything.
         * Readers just read the value, They don't write.
         */
        printf("Reader%s : Read value from buffer : %d\n", (char *) args,
                buffer);
        sleep(1);

        sem_wait(&mutex);
        readers--;
        if (!readers) {
            sem_post(&writer_lock);
        }
        sem_post(&mutex);
    }
    return NULL;
}

int main(void)
{
    pthread_t writer;
    pthread_t reader1, reader2;

    buffer = 0;     // Initialize the buffer value.
    readers = 0;    // Initially, there wont be any reader.

    sem_init(&writer_lock, 0, 1);
    sem_init(&mutex, 0, 1);

    pthread_create(&writer, NULL, writer_func, NULL);
    pthread_create(&reader1, NULL, reader_func, "1");
    pthread_create(&reader2, NULL, reader_func, "2");

    pthread_join(writer, NULL);
    pthread_join(reader1, NULL);
    pthread_join(reader2, NULL);

    return 0;
}
