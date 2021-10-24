#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX 5

sem_t forks[MAX];
sem_t footman;
int *philosopher_count;

void think(int philosopher)
{
    printf("Philosopher%d thinking.\n", philosopher);
    return;
}

void eat(int philosopher)
{
    printf("Philosopher%d eating.\n", philosopher);
    return;
}

void get_forks(int philosopher)
{
    // 'footman' allows maximum of 'MAX-1' philosophers.
    sem_wait(&footman);

    // take left fork.
    sem_wait(&forks[philosopher]);

    // take right fork.
    sem_wait(&forks[(philosopher+1) % MAX]);
    return;
}

void put_forks(int philosopher)
{
    // put left fork.
    sem_post(&forks[philosopher]);

    // put right fork.
    sem_post(&forks[(philosopher+1) % MAX]);

    sem_post(&footman);
    return;
}

void *func(void *args)
{
    int philosopher = *(int *)args;

    while (1) {
        think(philosopher);
        get_forks(philosopher);
        eat(philosopher);
        put_forks(philosopher);
    }

    return NULL;
}

int main(void)
{
    pthread_t philosophers[MAX];
    philosopher_count = malloc(MAX * sizeof(int));

    for (int i=0; i<MAX; i++) {
        sem_init(&forks[i], 0, 1);
    }

    sem_init(&footman, 0, MAX-1);

    for (int i=0; i<MAX; i++) {
        philosopher_count[i] = i;
    }

    for (int i=0; i<MAX; i++) {
        pthread_create(&philosophers[i], NULL, func, &philosopher_count[i]);
    }

    for (int i=0; i<MAX; i++) {
        pthread_join(philosophers[i], NULL);
    }

    for (int i=0; i<MAX; i++) {
        sem_destroy(&forks[i]);
    }

    sem_destroy(&footman);

    return 0;
}
