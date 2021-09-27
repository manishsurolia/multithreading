#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include "list.h"
#include <unistd.h>
#define BILLION 1000000000L

typedef struct thread_data
{
    list *l;
    unsigned int start_index;
} thread_data;

void * insert_and_delete_ten_nodes(void *arg)
{
    thread_data * p = (thread_data *)arg;

    if (!p) {
        return NULL;
    }

    for (int i = 0; i < 10; i++) {
        sleep(1);
        add_node(p->l, (p->start_index + i));
    }

    for (int i = 0; i < 10; i++) {
        sleep(1);
        delete_node(p->l, (p->start_index+i));
    }

    return NULL;
}

void * print_list(void *args)
{
    thread_data * p = (thread_data *)args;

    if (!p) {
        return NULL;
    }

    while(1) {
        print(p->l);
    }

    return NULL;
}

int main(void)
{
    unsigned int thread_count;
    pthread_t *threads;
    pthread_t print_thread;
    thread_data * d = NULL;
    uint64_t total_time = 0;
    struct timespec tstart={0,0}, tend={0,0};

    list * l = (list *) malloc (sizeof(list));
    if (!l) {
        perror("List creation filed!!\n");
        return 0;
    }
    thread_data print_data = {l, 0};

    list_init(l);
    printf("How many threads you want to create? ");
    scanf("%d", &thread_count);

    threads = (pthread_t *) malloc (sizeof(pthread_t) * thread_count);
    d = (thread_data *) malloc (sizeof (thread_data) * thread_count);

    unsigned int start_index = 0;
    for (unsigned int i=0; i<thread_count; i++) {
        d[i].l = l;
        d[i].start_index = start_index;
        start_index += 10; // Each thread adds just 10 nodes in the list.
    }

    clock_gettime(CLOCK_MONOTONIC, &tstart);
    for (unsigned int i=0; i<thread_count; i++) {
        pthread_create(&threads[i], NULL, insert_and_delete_ten_nodes, &d[i]);
    }
    pthread_create(&print_thread, NULL, print_list, &print_data);

    for (unsigned int i=0; i<thread_count; i++) {
        pthread_join(threads[i], NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &tend);
    total_time = (BILLION * (tend.tv_sec - tstart.tv_sec)) +
        (tend.tv_nsec - tstart.tv_nsec);
    printf("Time Taken : %lu nano-seconds.\n", total_time);

    print(l);

    return 0;
}
