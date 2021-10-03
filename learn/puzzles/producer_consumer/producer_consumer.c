#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * Buffer to produce and consume data.
 * Shared between threads.
 * Will be used in circular fashion.
 */
unsigned int *buffer;
unsigned int buffer_size;

unsigned int producer_index;
unsigned int consumer_index;

sem_t producer_resources, producer_mutex;
sem_t consumer_resources, consumer_mutex;

void *producer_func(void *args)
{
    while (1) {
        sem_wait(&producer_resources);
        sem_wait(&producer_mutex);
        buffer[producer_index] = rand() % 100;
        printf("Producer%s : Data %d added to the buffer.\n", (char *)args,
                buffer[producer_index]);
        producer_index = (producer_index + 1) % buffer_size;
        sem_post(&producer_mutex);
        sem_post(&consumer_resources);
    }
    return NULL;
}

void *consumer_func(void *args)
{
    while (1) {
        sem_wait(&consumer_resources);
        sem_wait(&consumer_mutex);
        printf("Consumer%s : Data %d consumed from the buffer.\n", (char *)args,
                buffer[consumer_index]);
        consumer_index = (consumer_index + 1) % buffer_size;
        sem_post(&consumer_mutex);
        sleep(1);
        sem_post(&producer_resources);
    }
    return NULL;
}

int main(void)
{
    /*
     * Three prodcers and two consumers.
     */
    pthread_t producer1, producer2, producer3;
    pthread_t consumer1, consumer2;
    producer_index = consumer_index = 0;

    printf("What is the size of Buffer\n");
    scanf("%d", &buffer_size);

    buffer = (unsigned int *) malloc (sizeof(int) * buffer_size);
    /*
     * Initially producer has all the resources, but consumer does not have any.
     * Once producer adds data in the buffer, resources will be added to the
     * consumer semaphore, and vice-versa.
     */
    sem_init(&producer_resources, 0, buffer_size);
    sem_init(&consumer_resources, 0, 0);

    /*
     * Initlialize the mutexes.
     */
    sem_init(&consumer_mutex, 0, 1);
    sem_init(&producer_mutex, 0, 1);

    pthread_create(&producer1, NULL, producer_func, "1");
    pthread_create(&producer2, NULL, producer_func, "2");
    pthread_create(&producer3, NULL, producer_func, "3");

    pthread_create(&consumer1, NULL, consumer_func, "1");
    pthread_create(&consumer2, NULL, consumer_func, "2");

    pthread_join(producer1, NULL);
    pthread_join(producer2, NULL);
    pthread_join(producer3, NULL);

    pthread_join(consumer1, NULL);
    pthread_join(consumer2, NULL);

    sem_destroy(&producer_resources);
    sem_destroy(&consumer_resources);
    sem_destroy(&producer_mutex);
    sem_destroy(&consumer_mutex);

    return 0;
}
