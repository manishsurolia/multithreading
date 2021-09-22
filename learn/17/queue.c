#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "queue.h"

void enqueue(queue *q, int data)
{
    if (!q) {
        return; // List not initialized, just reutrn.
    }

    node * new = (node *) malloc (sizeof(node));
    if (!new) {
        perror ("malloc failed\n");
        return; // malloc failed, just return.
    }
    new->value = data;
    new->next = NULL;

    pthread_mutex_lock(&q->tail_lock);
    q->tail->next = new;
    q->tail = new;
    pthread_mutex_unlock(&q->tail_lock);

    return;
}

int dequeue(queue *q, int *value)
{
    node * temp;

    if (!q || !q->head || !q->tail) {
        return -1; // List not initialized, just return error.
    }

    pthread_mutex_lock(&q->head_lock);

    /*
     * Head always points to the dummy node, so we need to dequeue the next
     * from dummy.
     */

    temp = q->head->next;
    if (!temp) {
        // Queue is empty. Release the lock and return.
        pthread_mutex_unlock(&q->head_lock);
        return -1;
    }

    *value = temp->value;
    q->head->next = temp->next;

    pthread_mutex_unlock(&q->head_lock);

    free(temp);

    return 0;
}

void print(queue *q)
{
    node *temp;
    if (!q || !q->head || !q->tail) {
        return;
    }

    pthread_mutex_lock(&q->head_lock);
    pthread_mutex_lock(&q->tail_lock);
    temp = q->head->next; // head is just dummy, start from the next, till end.
    printf("Queue :");
    while (temp) {
        printf(" %d", temp->value);
        temp = temp->next;
    }
    printf("\n");
    pthread_mutex_unlock(&q->head_lock);
    pthread_mutex_unlock(&q->tail_lock);

    return;
}

void queue_init (queue *q)
{
    if (!q) {
        // Queue is NULL, Just returning.
        return;
    }

    /*
     * At least one node (dummy node) should be present in the queue.
     */
    node *p = (node *) malloc (sizeof(node));
    if (!p) {
        perror("malloc failed!\n");
    }

    p->next = NULL;
    q->head = p;
    q->tail = p;

    pthread_mutex_init(&q->head_lock, NULL);
    pthread_mutex_init(&q->tail_lock, NULL);

    return;
}
