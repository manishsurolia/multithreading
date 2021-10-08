#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

int total_threads;

int count;
sem_t mutex;

int release = 0;

void barrier(void)
{
    int local_sense = 1; // Opposite of initial value of global 'release'.

    sem_wait(&mutex);
    count++;
    sem_post(&mutex);

    if (count == total_threads) {
        release = local_sense;
    }

    while (release != local_sense);
    return;
}

void * func(void *args)
{

    printf("Executed Rendezous code.\n");
    barrier();
    // Critical point.
    printf("Executed Critical point.\n");

    return NULL;
}


int main(void)
{
    pthread_t *threads;
    sem_init(&mutex, 0, 1);

    count = 0;
    release = 0;

    printf("How many threads you want to create.\n");
    scanf("%d", &total_threads);

    threads = (pthread_t *) malloc (sizeof(pthread_t) * total_threads);

    for (int i=0; i<total_threads; i++) {
        pthread_create(&threads[i], NULL, func, NULL);
    }

    for (int i=0; i<total_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    sem_destroy(&mutex);
    return 0;
}
