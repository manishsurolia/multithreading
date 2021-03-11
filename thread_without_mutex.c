/*
 * In this program, you will see that both the thread are executing together in
 * the critical section, which we dont want.
 *
 * gcc thread_without_mutex.c -lpthread
 * ./a.out
 */
#include <pthread.h>
#include <stdio.h>

void *func(void *p)
{
    unsigned int i;
    printf("Thread %lu started. Entering into cirtical secion of the program\n",
            pthread_self());
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
    return NULL;
}

int main(void)
{
    pthread_t tid1, tid2;

    if (!pthread_create(&tid1, NULL, &func, NULL)) {
       printf("Thread creation sucessful."); 
    } else {
       printf("Thread creation failed."); 
    }

    if(!pthread_create(&tid2, NULL, &func, NULL)) {
        printf("Thread creation sucessful.\n");
    } else {
        printf("Thread creation failed.\n");
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}
