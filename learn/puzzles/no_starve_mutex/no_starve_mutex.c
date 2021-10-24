#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include <unistd.h>

/*
 * barrier related data.
 */
int counter;
sem_t mutex;
sem_t turnstile1;
sem_t turnstile2;
int n; // number of threads.

void barrier(void)
{
    // Phase-1 barrier.
    sem_wait(&mutex);
    counter++;
    sem_post(&mutex);

    if (counter == n) {
        for (int i=0; i<n; i++) {
            sem_post(&turnstile1);
        }
    }

    sem_wait(&turnstile1);

    // Phase-2 barrier.
    sem_wait(&mutex);
    counter--;
    sem_post(&mutex);

    if (counter == 0) {
        for (int i=0; i<n; i++) {
            sem_post(&turnstile2);
        }
    }

    sem_wait(&turnstile2);

    return;
}


void *func(void *args)
{
    while(1) {
        sleep(1);
        barrier();
        printf("Executing code for : %s\n", (char *)args);
    }
    return NULL;
}

int main(void)
{
    pthread_t t1, t2, t3, t4, t5, t6, t7, t8, t9, t10;

    /*
     * barrier related intialisation.
     */
    sem_init(&mutex, 0, 1); // Binary semaphore, mutex.
    sem_init(&turnstile1, 0, 0);
    sem_init(&turnstile2, 0, 0);
    counter = 0;
    n = 10; // number of threads

    pthread_create(&t1, NULL, func, "T1");
    pthread_create(&t2, NULL, func, "T2");
    pthread_create(&t3, NULL, func, "T3");
    pthread_create(&t4, NULL, func, "T4");
    pthread_create(&t4, NULL, func, "T5");
    pthread_create(&t4, NULL, func, "T6");
    pthread_create(&t4, NULL, func, "T7");
    pthread_create(&t4, NULL, func, "T8");
    pthread_create(&t4, NULL, func, "T9");
    pthread_create(&t4, NULL, func, "T10");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_join(t5, NULL);
    pthread_join(t6, NULL);
    pthread_join(t7, NULL);
    pthread_join(t8, NULL);
    pthread_join(t9, NULL);
    pthread_join(t10, NULL);

    sem_destroy(&mutex);
    sem_destroy(&turnstile1);
    sem_destroy(&turnstile2);
    return 0;
}
