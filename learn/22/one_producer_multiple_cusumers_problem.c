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
    for (unsigned int i=0; i<50; i++) {
        pthread_mutex_lock(&lock);
        while (flag == TRUE) {
            pthread_cond_wait(&cond, &lock);
        }
        buffer = i;
        flag = TRUE;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *consumer_func1(void *arg)
{
    while (1) {
        pthread_mutex_lock(&lock);
        while (flag == FALSE) {
            pthread_cond_wait(&cond, &lock);
        }
        printf("Consumer1 : Got value %d from the buffer\n", buffer);
        flag = FALSE;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *consumer_func2(void *arg)
{
    while (1) {
        pthread_mutex_lock(&lock);
        while (flag == FALSE) {
            pthread_cond_wait(&cond, &lock);
        }
        printf("Consumer2 : Got value %d from the buffer\n", buffer);
        flag = FALSE;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(void)
{
    pthread_t producer, consumer1, consumer2;
    flag = FALSE; // Initially the flag is 0, Means the buffer is empty.

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create (&producer, NULL, producer_func, NULL);
    pthread_create (&consumer1, NULL, consumer_func1, NULL);
    pthread_create (&consumer2, NULL, consumer_func2, NULL);

    pthread_join (producer, NULL);
    pthread_join (consumer1, NULL);
    pthread_join (consumer2, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
}
