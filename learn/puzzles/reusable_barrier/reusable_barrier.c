#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

sem_t mutex;
sem_t turnstile1;
sem_t turnstile2;
int n; // max number of threads
int count;

void barrier(void)
{
    // Phase-1 barrier
    sem_wait(&mutex);
    count++;
    sem_post(&mutex);

    if (count == n) {
        for (int i=0; i < n; i++) {
            sem_post(&turnstile1);
        }
    }

    sem_wait(&turnstile1);

    // Phase-2 barrier
    sem_wait(&mutex);
    count--;
    sem_post(&mutex);

    if (count == 0) {
        for (int i=0; i < n; i++) {
            sem_post(&turnstile2);
        }
    }

    sem_wait(&turnstile2);

    return;
}

/*
 * Symmetric code for all the threads.
 */
void * func(void *args)
{
    printf("Executed rendezvous code.\n");
    barrier();
    printf("Executed Critical point-1.\n");
    barrier();
    printf("Executed Critical point-2.\n");
    barrier();
    printf("Executed Critical point-3.\n");
    barrier();
    printf("Executed Critical point-4.\n");
    barrier();
    printf("Executed Critical point-5.\n");
    return NULL;
}

int main(void)
{
    pthread_t *threads;
    unsigned int temp;

    sem_init(&turnstile1, 0, 0);
    sem_init(&turnstile2, 0, 0);
    sem_init(&mutex, 0 , 1);

    printf("How many thread, you want to create? ");
    scanf("%d", &temp);

    threads = (pthread_t *) malloc (sizeof(pthread_t) * temp);
    n = temp;                   // Set number of thread in 'n'
    count = 0;                  // Initially, Make the global 'count' as 0.

    for (unsigned int i=0; i<temp; i++) {
        pthread_create(&threads[i], NULL, func, NULL);
    }

    for (unsigned int i=0; i<temp; i++) {
        pthread_join(threads[i] , NULL);
    }

    sem_destroy(&turnstile1);
    sem_destroy(&turnstile2);
    sem_destroy(&mutex);

    return 0;
}
