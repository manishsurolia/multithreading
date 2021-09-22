#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <pthread.h>

typedef struct node
{
    int value;
    struct node * next;
} node;

typedef struct list
{
    node * head;
    node * tail;
    pthread_mutex_t head_lock, tail_lock;
} queue;

void queue_init (queue *p);
void enqueue(queue *q, int data);
int dequeue(queue *q, int *value);
void print(queue *q);

#endif //__QUEUE_H__
