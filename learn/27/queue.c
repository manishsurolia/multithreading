#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX 2

int buffer[MAX];
int write_index;
int read_index;

sem_t writer_sem; //Initially, this will have MAX number of resources(defined above).
sem_t reader_sem; //Initially, it has 0 number of resrouces.

sem_t lock; //binary semaphore.

void *producer(void *arg)
{
    for (int i=0; i<10; i++) {
        sem_wait(&writer_sem);
        // Critical section start.
        sem_wait(&lock); //Take lock using binary semaphore.
        buffer[write_index] = i;
        printf("%s : Written value to buffer : %d\n", (char *)arg, i);
        write_index = (write_index + 1) % MAX;
        sem_post(&lock); //Release the lock.
        // Critical Section end.
        sem_post(&reader_sem);
    }
    return NULL;
}

void *consumer(void *arg)
{
    int temp;
    while (1) {
        sem_wait(&reader_sem);
        //Critical section start.
        sem_wait(&lock); //Take lock using binary semaphore.
        temp = buffer[read_index];
        printf("%s : Read value from buffer : %d\n", (char *)arg, temp);
        read_index = (read_index + 1) % MAX;
        sem_post(&lock); //Release the lock.
        // Critical Section end.
        sem_post(&writer_sem);
    }
    return NULL;
}

int main(void)
{
    pthread_t producer_thread1, consumer_thread1;
    pthread_t producer_thread2, consumer_thread2;
    sem_init(&writer_sem, 0, MAX);
    sem_init(&reader_sem, 0, 0);
    sem_init(&lock, 0, 1); // lock(binary semaphore for lock around critical section).
    write_index = read_index = 0;

    pthread_create(&producer_thread1, NULL, producer, "P1");
    pthread_create(&consumer_thread1, NULL, consumer, "C1");
    pthread_create(&producer_thread2, NULL, producer, "P2");
    pthread_create(&consumer_thread2, NULL, consumer, "C2");

    pthread_join(producer_thread1, NULL);
    pthread_join(consumer_thread2, NULL);
    pthread_join(producer_thread1, NULL);
    pthread_join(consumer_thread2, NULL);

    sem_destroy(&writer_sem);
    sem_destroy(&reader_sem);

    return 0;
}
