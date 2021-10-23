#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

/*
 * Barrier related data.
 */
sem_t trunstile;
sem_t mutex;                    // To protect writes to 'count'.
unsigned int n;                 // Total number of threads
unsigned int count;

/*
 * Barrier function.
 */
void barrier(void)
{
    sem_wait(&mutex);
    count++;                    // Critical Section.
    sem_post(&mutex);

    if (count == n) {
        sem_post(&trunstile);
    }

    sem_wait(&trunstile);
    sem_post(&trunstile);

    return;
}

/*
 * Symmetric code for all the threads.
 */
void * thread_func(void *args)
{
    printf("Executed rendezvous code.\n");
    barrier();
    printf("Executed Critical point.\n");
    return 0;
}

int main(void)
{
    pthread_t *threads;
    unsigned int temp;

    sem_init(&trunstile, 0, 0); // Barrier.
    sem_init(&mutex, 0 , 1);    // Lock, Binary semaphore.

    printf("How many thread, you want to create? ");
    scanf("%d", &temp);

    threads = (pthread_t *) malloc (sizeof(pthread_t) * temp);
    n = temp;                   // Set number of thread in 'n'
    count = 0;                  // Initially, Make the global 'count' as 0.

    for (unsigned int i=0; i<temp; i++) {
        pthread_create(&threads[i], NULL, thread_func, NULL);
    }

    for (unsigned int i=0; i<temp; i++) {
        pthread_join(threads[i] , NULL);
    }

    sem_destroy(&trunstile);
    sem_destroy(&mutex);

    return 0;
}

