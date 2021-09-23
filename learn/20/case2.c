#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

unsigned int flag;

pthread_mutex_t lock;
pthread_cond_t cond;

void * func1(void *arg)
{
    pthread_mutex_lock(&lock);
    while (flag == FALSE) {
        printf("T1 : Calling wait to release lock and sleep.\n");
        pthread_cond_wait(&cond, &lock);
        printf("T1 : Got signal from the other thread. Rechecking the flag.\n");
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

void *func2(void *arg)
{
    printf("T2 : Trying to acquire lock.\n");
    pthread_mutex_lock(&lock);
    flag = TRUE;
    printf("T2 : Acquired lock. Making flag as TRUE. Returning.\n");
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&cond);
}

int main(void)
{
    pthread_t thread1, thread2;
    flag = FALSE;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_create(&thread2, NULL, func2, NULL);
    sleep(1); // Fooling the scheduler to start thread2 first.
    pthread_create(&thread1, NULL, func1, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
}


