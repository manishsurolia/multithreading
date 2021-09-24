/*
 * Cirular Queue Implementation.
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define MAX 100

int buffer[MAX];
int head;
int tail;

pthread_mutex_t lock;
pthread_cond_t enqueue_condition;
pthread_cond_t dequeue_condition;


void enqueue(int data)
{
    pthread_mutex_lock(&lock);
    while (((tail + 1) % MAX) == head) {
        // Queue is Full. wait for enqueue signal from readers.
        pthread_cond_wait(&enqueue_condition, &lock);
    }
    buffer[tail] = data;
    tail = (tail + 1) % MAX;
    printf("Enqueued element %d\n",data);
    pthread_cond_signal(&dequeue_condition);
    pthread_mutex_unlock(&lock);
    return;
}

int dequeue(void)
{
    int element;
    pthread_mutex_lock(&lock);
    while (head == tail) {
        pthread_cond_wait(&dequeue_condition, &lock);
    }
    element = buffer[head];
    head = (head + 1) % MAX;
    printf("Dequeued element : %d\n",element);
    pthread_cond_signal(&enqueue_condition);
    pthread_mutex_unlock(&lock);
    return element;
}

void * producer(void *arg)
{
    int element = 0;
    for (int i=0; i<10; i++) {
        enqueue(rand() % 10);
        element++;
    }
    return NULL;
}

void * consumer (void *arg)
{
    while (1) {
        dequeue();
    }
    return NULL;
}


int main(void)
{
    pthread_t producer_thread1, consumer_thread1;
    pthread_t producer_thread2, consumer_thread2;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&enqueue_condition, NULL);
    pthread_cond_init(&dequeue_condition, NULL);

    head = tail = 0; // Queue is empty.

    pthread_create(&producer_thread1, NULL, producer, NULL);
    pthread_create(&consumer_thread1, NULL, consumer, NULL);
    pthread_create(&producer_thread2, NULL, producer, NULL);
    pthread_create(&consumer_thread2, NULL, consumer, NULL);

    pthread_join(producer_thread1, NULL);
    pthread_join(consumer_thread1, NULL);
    pthread_join(producer_thread2, NULL);
    pthread_join(consumer_thread2, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&enqueue_condition);
    pthread_cond_destroy(&dequeue_condition);

    return 0;
}
