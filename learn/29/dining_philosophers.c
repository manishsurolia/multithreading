#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX 5

sem_t forks[MAX];
int philosopher_count[MAX] = {0, 1, 2 ,3 ,4};

void think(void *philosopher)
{
    // Just a dummy function
}

void eat(void *philosopher)
{
    // Just a dummy function
}

void get_forks(void *arg)
{
    int philosopher = *(int *)arg;
    sem_wait(&forks[philosopher]); // Wait for left fork.
    printf("P%d : Acquired F%d and waiting for F%d\n", *(int *)arg, *(int *)arg,
            (*(int *)arg+1)%MAX);
    sem_wait(&forks[(philosopher+1)%MAX]); // Wait for right fork.
}

void put_forks(void *arg)
{
    int philosopher = *(int *)arg;
    sem_post(&forks[philosopher]); // Release left fork.
    sem_post(&forks[(philosopher+1)%MAX]); // Release right fork.
}

void *philosopher (void *arg)
{
    // Each philosopher is thinking and eating infinitely
    while (1) {
        think(arg);
        get_forks(arg);
        eat(arg);
        put_forks(arg);
    }
}

int main(void)
{
    pthread_t philosophers[MAX];
    for (int i = 0; i < MAX; i++) {
        sem_init(&forks[i], 0 , 1); // Making each fork(resource) as binary semaphore as only one philosopher can use it at a time.
    }

    for (int i = 0; i < MAX; i++) {
        pthread_create(&philosophers[i], NULL, philosopher,
                &philosopher_count[i]);
    }

    for (int i = 0; i < MAX; i++) {
        pthread_join(philosophers[i], NULL);
    }

    return 0;
}
