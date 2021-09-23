#include <stdio.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0

unsigned int buffer; // Written by producer, and read by consumer thread.
int flag; // FALSE => Buffer is empty, TRUE => Buffer has data to be read.

pthread_mutex_t lock;
pthread_cond_t cond;

void * producer_func (void *arg)
{
    pthread_mutex_lock(&lock);
    for (unsigned int i=0; i<10; i++) {
        while (flag == TRUE) {
            pthread_cond_wait(&cond, &lock);
        }
        printf("Producer : Writing %d value to the buffer\n", i);
        buffer = i;
        flag = TRUE;
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

void *consumer_func(void *arg)
{
    pthread_mutex_lock(&lock);
    for (unsigned int i=0; i<10; i++) {
        while (flag == FALSE) {
            pthread_cond_wait(&cond, &lock);
        }
        printf("Consumer : Got value %d from the buffer\n", buffer);
        flag = FALSE;
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(void)
{
    pthread_t producer, consumer;
    flag = FALSE; // Initially the flag is 0, Means the buffer is empty.

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create (&producer, NULL, producer_func, NULL);
    pthread_create (&consumer, NULL, consumer_func, NULL);

    pthread_join (producer, NULL);
    pthread_join (consumer, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
}
