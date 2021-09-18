#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>

#define BILLION 1000000000L

typedef struct thread_data {
    char thread_name[32];
    pthread_t thread_id;
} thread_data;

unsigned int counter = 0; // Global counter.
pthread_mutex_t lock; // Lock for the above counter.

void *func(void *p)
{
    for (unsigned int i=0; i<10000000; i++) {
        /*
         * Below line is Critical section of this function, as it is trying to
         * update the global variable.
         */
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(void)
{
    unsigned int thread_count;
    thread_data *threads;
    pthread_mutex_init(&lock, NULL); // Need to have a global lock around critical section.
    struct timespec tstart={0,0}, tend={0,0};
    uint64_t total_time = 0;

    printf("How many threads, do you want to create? ");
    scanf("%d",&thread_count);
    threads = (thread_data *) malloc (sizeof(thread_data) * thread_count);

    /* Create thread name and start the thread */
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    for (unsigned int i = 0; i < thread_count; i++) {
        snprintf(threads[i].thread_name, 32, "T%d", i);
        pthread_create(&threads[i].thread_id, NULL, func,
                &threads[i].thread_name);
    }

    for (unsigned int i = 0; i < thread_count; i++) {
        pthread_join(threads[i].thread_id, NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &tend);
    total_time = (BILLION * (tend.tv_sec - tstart.tv_sec)) +
        (tend.tv_nsec - tstart.tv_nsec);

    printf("counter = %d, in %lu milli-seconds\n", counter,
            (total_time/1000000));

    return 0;
}
