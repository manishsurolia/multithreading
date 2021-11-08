#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

sem_t sem_a;
sem_t sem_b;

void *thread_a(void *args)
{
    while (1) {
        sem_wait(&sem_b);
        printf("A");
        sem_post(&sem_a);
    }
    return NULL;
}

void *thread_b(void *args)
{
    while (1) {
        sem_wait(&sem_a);
        sleep(1);
        printf("B");
        fflush(stdout);
        sem_post(&sem_b);
    }
    return NULL;
}
int main(void)
{
    pthread_t t1, t2;

    sem_init(&sem_b, 0, 1);
    sem_init(&sem_a, 0, 0);

    pthread_create(&t1, NULL, thread_a, NULL);
    pthread_create(&t2, NULL, thread_b, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
