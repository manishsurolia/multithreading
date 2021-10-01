#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#define TOTAL_WRITERS 1
#define TOTAL_READERS 5

unsigned int val;
sem_t mutex; // Lock for mutual exclusion to modify 'val'. (critical section).
sem_t multiplex; // To decide how many readers can read the 'val' at a time.

void * reader(void *args)
{
    unsigned int temp;

    /*
     * Not a critical section, Just limiting the number of semaphores to access
     * 'val'.
     */
    sem_wait(&multiplex);
    temp = val;
    sem_post(&multiplex);
    printf("Reader(%ld) : Read the value : %d\n", pthread_self(), temp);
}

void *writer(void *args)
{
    /*
     * This is critical section. Val is getting modified. Hence, using Mutex.
     */
    sem_wait(&mutex);
    val = rand() % 100;
    sem_post(&mutex);
}

int main(void)
{
    val = 0;

    pthread_t readers[TOTAL_READERS];
    pthread_t writers[TOTAL_WRITERS];

    /*
     * Binary Semaphore (Mutex) for critical section.
     * Will be used for writing by one or many writers.
     */
    sem_init(&mutex, 0, 1);

    /*
     * We want to allow only 2 reader threads to read 'val' at a time.
     * Not for critical section, No update to 'val', Only for read purpose.
     */
    sem_init(&multiplex, 0, 2);

    for (unsigned int i=0; i<TOTAL_WRITERS; i++) {
        pthread_create(&writers[i], NULL, writer, NULL);
    }

    for (unsigned int i=0; i<TOTAL_READERS; i++) {
        pthread_create(&readers[i], NULL, reader, NULL);
    }

    for (unsigned int i=0; i<TOTAL_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    for (unsigned int i=0; i<TOTAL_READERS; i++) {
        pthread_join(readers[i], NULL);
    }

    return 0;
}
