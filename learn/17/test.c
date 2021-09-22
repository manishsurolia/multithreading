#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include "queue.h"

#define BILLION 1000000000L

typedef struct thread_data
{
    queue *queue;
    unsigned int start_index;
} thread_data;

void * insert_and_delete_ten_nodes(void *arg)
{
    thread_data * p = (thread_data *)arg;
    int res = -1;

    if (!p) {
        return NULL;
    }

    for (int i = 0; i < 100; i++) {
        enqueue(p->queue, (p->start_index + i));
    }

    for (int i = 0; i < 100; i++) {
        dequeue(p->queue, &res);
    }

    return NULL;
}

int main(void)
{
    unsigned int thread_count;
    pthread_t *threads;
    thread_data * d = NULL;
    uint64_t total_time = 0;
    struct timespec tstart={0,0}, tend={0,0};

    queue * q = (queue *) malloc (sizeof(queue));
    if (!q) {
        perror("Queue creation filed!!\n");
        return 0;
    }

    queue_init(q);
    printf("How many threads you want to create? ");
    scanf("%d", &thread_count);

    threads = (pthread_t *) malloc (sizeof(pthread_t) * thread_count);
    d = (thread_data *) malloc (sizeof (thread_data) * thread_count);

    unsigned int start_index = 0;
    for (unsigned int i=0; i<thread_count; i++) {
        d[i].queue = q;
        d[i].start_index = start_index;
        start_index += 100; // Each thread adds just 10 nodes in the queue.
    }

    clock_gettime(CLOCK_MONOTONIC, &tstart);
    for (unsigned int i=0; i<thread_count; i++) {
        pthread_create(&threads[i], NULL, insert_and_delete_ten_nodes, &d[i]);
    }

    for (unsigned int i=0; i<thread_count; i++) {
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &tend);
    total_time = (BILLION * (tend.tv_sec - tstart.tv_sec)) +
        (tend.tv_nsec - tstart.tv_nsec);
    printf("Time Taken : %lu nano-seconds.\n", total_time);

    print(q);

    return 0;
}
