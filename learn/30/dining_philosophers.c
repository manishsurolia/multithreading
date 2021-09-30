#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX 5

sem_t forks[MAX];
sem_t lock;
int philosopher_count[MAX] = {0, 1, 2 ,3 ,4};

void think(void)
{
    // Just a dummy function
}

void eat(void)
{
    // Just a dummy function
}

void get_forks(void *arg)
{
    int philosopher = *(int *)arg;
    sem_wait(&lock);                        // Take global lock.
    sem_wait(&forks[philosopher]);          // Take left fork.
    sem_wait(&forks[(philosopher+1)%MAX]);  // Take right fork.
    sem_post(&lock);                        // Release global lock.
}

void put_forks(void *arg)
{
    int philosopher = *(int *)arg;
    sem_post(&forks[philosopher]);          // Put left fork.
    sem_post(&forks[(philosopher+1)%MAX]);  // Put right fork.
    return;
}

void *philosopher (void *arg)
{
    int philosopher = *(int *) arg;

    /*
     * Each philosopher will think and eat 1 Millon times.
     */
    for (unsigned int i = 0; i < 1000000; i++) {
        think();
        get_forks(arg);
        eat();
        put_forks(arg);
    }
    printf("P%d : DONE\n", philosopher);
}

int main(void)
{
    pthread_t philosophers[MAX];
    for (int i = 0; i < MAX; i++) {
        /*
         * Making each fork(resource) as binary semaphore as only one
         * philosopher can use it at a time.
         */
        sem_init(&forks[i], 0 , 1);
    }

    /*
     * Global lock to be acquired by each philosopher before getting both left
     * and right side forks.
     *
     * NOTE : We don't use (and should not, to avoid deadlocks) this lock for
     * releasing the forks.
     */
    sem_init(&lock, 0, 1);

    for (int i = 0; i < MAX; i++) {
        pthread_create(&philosophers[i], NULL, philosopher,
                &philosopher_count[i]);
    }

    for (int i = 0; i < MAX; i++) {
        pthread_join(philosophers[i], NULL);
    }

    for (int i = 0; i < MAX; i++) {
        sem_destroy(&forks[i]);
    }

    sem_destroy(&lock);
    return 0;
}
