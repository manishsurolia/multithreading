#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX 5

int buffer[MAX];
int write_index;
int read_index;

sem_t writer_sem; //Initially, this will have MAX number of resources(defined above).
sem_t reader_sem; //Initially, it has 0 number of resrouces.

void *producer(void *arg)
{
    for (int i=0; i<10; i++) {
        sem_wait(&writer_sem);
        // Critical section start.
        buffer[write_index] = i;
        printf("Written value to buffer : %d\n", i);
        write_index = (write_index + 1) % MAX;
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
        temp = buffer[read_index];
        printf("Read value from buffer : %d\n", temp);
        read_index = (read_index + 1) % MAX;
        // Critical Section end.
        sem_post(&writer_sem);
    }
    return NULL;
}

int main(void)
{
    pthread_t producer_thread, consumer_thread;
    sem_init(&writer_sem, 0, MAX);
    sem_init(&reader_sem, 0, 0);
    write_index = read_index = 0;

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    sem_destroy(&writer_sem);
    sem_destroy(&reader_sem);

    return 0;
}
