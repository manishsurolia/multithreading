#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX 5    // Maximum number of resources(buffers) available.

int buffer[MAX]; // Total buffers are 'MAX'

int fill_index;  // Which index to fill a new entry in buffer array.
int use_index;   // Which index to use the data in buffer array.

sem_t empty_sem; // Holds the count of empty buffers.
sem_t full_sem;  // Holds the count of full buffers.

void *producer(void *arg)
{
    for (int i=0; i<10; i++) {
        sem_wait(&empty_sem);
        // Critical section start.
        buffer[fill_index] = i;
        printf("%s : Produced value to buffer : %d\n", (char *)arg, i);
        fill_index = (fill_index + 1) % MAX;
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
        temp = buffer[use_index];
        printf("%s : Consumed value from buffer : %d\n", (char *)arg, temp);
        use_index = (use_index + 1) % MAX;
        // Critical Section end.
        sem_post(&empty_sem);
    }
    return NULL;
}

int main(void)
{
    pthread_t producer_thread, consumer_thread;
    sem_init(&empty_sem, 0, MAX);
    sem_init(&full_sem, 0, 0);
    fill_index = use_index = 0;

    pthread_create(&producer_thread, NULL, producer, "Producer");
    pthread_create(&consumer_thread, NULL, consumer, "Consumer");

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    sem_destroy(&empty_sem);
    sem_destroy(&full_sem);

    return 0;
}
