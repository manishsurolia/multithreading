#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#define MAX 10

unsigned int buffer[MAX]; // Buffer can hold max of 10 integers.
int count; // Count represents how much buffer is full. 0 = EMPTY, MAX = FULL. 

pthread_mutex_t lock;
pthread_cond_t write_cond;
pthread_cond_t read_cond;

void *producer_func1(void *arg)
{
    for (unsigned int i = 0; i < 50; i++)
    {
        pthread_mutex_lock(&lock);

        while (count == MAX) {
            count--;
            pthread_cond_wait(&write_cond, &lock);
        }

        buffer[count%MAX] = i;
        count++;

        pthread_cond_signal(&read_cond);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *consumer_func1(void *arg)
{
    while(1) {
        pthread_mutex_lock(&lock);

        while (count < 0) {
            count++;
            pthread_cond_wait(&read_cond, &lock);
        }

        printf("Got the value from buffer : %d\n", buffer[count%MAX]);
        count--;

        pthread_cond_signal(&write_cond);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(void)
{
    count = 0; // Buffer is empty.
    pthread_t consumer1, producer1;

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&read_cond, NULL);
    pthread_cond_init(&write_cond, NULL);

    pthread_create(&producer1, NULL, producer_func1, NULL);
    pthread_create(&consumer1, NULL, consumer_func1, NULL);

    pthread_join(producer1, NULL);
    pthread_join(consumer1, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&write_cond);
    pthread_cond_destroy(&read_cond);

    return 0;
}
