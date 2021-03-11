/*
 * In this program, you will see that both threads are executing one by one
 * into the critical section.
 *
 * Also, note that we have created the lock before creating any thread and
 * passed the lock pointer in pthread_create() function. 
 *
 * gcc thread_without_mutex.c -lpthread
 * ./a.out
 */
#include <pthread.h>
#include <stdio.h>

void *func(void *p)
{
    unsigned int i;
    pthread_mutex_lock(p);
    printf("Thread %lu started. Entering into a cirtical secion of the"
            " program\n", pthread_self());
    for (i = 0; i<0xFFFFFFFF; i++) {
        /*
         * Just a dummy loop. But think of this as a critical code which needs
         * to be accessed only by one thread at a time.
         *
         * Example of a critical section : updating a global variable.
         */
    }
    printf("Thread %lu has ended. Moving out of the critical section of the"
            " program.\n",pthread_self());
    pthread_mutex_unlock(p);
    return NULL;
}

int main(void)
{
    pthread_t tid1, tid2;
    pthread_mutex_t lock;

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf ("Mutex init failed.");
        return 1;
    }

    if (pthread_create(&tid1, NULL, &func, &lock) != 0) {
       printf("Thread creation failed.");
       return 1;
    }

    if(pthread_create(&tid2, NULL, &func, &lock) != 0) {
        printf("Thread creation failed.\n");
        return 1;
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_mutex_destroy(&lock);

    return 0;
}
