#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX 2    // Maximum number of resources(buffers) available.

int buffer[MAX]; // Total buffers are 'MAX'

int fill_index;  // Which index to fill a new entry in buffer array.
int use_index;   // Which index to use the data in buffer array.

sem_t empty_sem; // Holds the count of empty buffers.
sem_t full_sem;  // Holds the count of full buffers.

sem_t lock;      // Lock to lock the critical section.

void *producer(void *arg)
{
    for (int i=0; i<10; i++) {
        sem_wait(&empty_sem);
        // Critical section start.
        sem_wait(&lock);
        buffer[fill_index] = i;
        printf("%s : Produced value to buffer : %d\n", (char *)arg, i);
        fill_index = (fill_index + 1) % MAX;
        sem_post(&lock);
        // Critical Section end.
        sem_post(&full_sem);
    }
    return NULL;
}

void *consumer(void *arg)
{
    int temp;
    while (1) {
        sem_wait(&full_sem);
        //Critical section start.
        sem_wait(&lock);
        temp = buffer[use_index];
        printf("%s : Consumed value from buffer : %d\n", (char *)arg, temp);
        use_index = (use_index + 1) % MAX;
        sem_post(&lock);
        // Critical Section end.
        sem_post(&empty_sem);
    }
    return NULL;
}

int main(void)
{
    pthread_t producer_thread1, consumer_thread1;
    pthread_t producer_thread2, consumer_thread2;
    sem_init(&empty_sem, 0, MAX);
    sem_init(&full_sem, 0, 0);
    sem_init(&lock, 0, 1); // Making it binary semaphore for lock purpose.
    fill_index = use_index = 0;

    pthread_create(&producer_thread1, NULL, producer, "Producer 1");
    pthread_create(&consumer_thread1, NULL, consumer, "Consumer 1");
    pthread_create(&producer_thread2, NULL, producer, "Producer 2");
    pthread_create(&consumer_thread2, NULL, consumer, "Consumer 2");

    pthread_join(producer_thread1, NULL);
    pthread_join(consumer_thread1, NULL);
    pthread_join(producer_thread2, NULL);
    pthread_join(consumer_thread2, NULL);

    sem_destroy(&empty_sem);
    sem_destroy(&full_sem);
    sem_destroy(&lock);

    return 0;
}
