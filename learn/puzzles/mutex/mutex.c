#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

unsigned int count;
sem_t mutex; // Lock for mutual exclusion to increment 'count'.

void * func(void *args)
{
    for (unsigned int i=0; i<1000; i++) {
        sem_wait(&mutex);
        count = count + 1;
        sem_post(&mutex);
    }
}

int main(void)
{
    count = 0;
    pthread_t thread_a, thread_b;
    sem_init(&mutex, 0 ,1);

    pthread_create(&thread_a, NULL, func, NULL);
    pthread_create(&thread_b, NULL, func, NULL);

    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);

    printf("Count : %d\n", count);

    return 0;
}
