#include <stdio.h>                                                                  
#include <pthread.h>
#include <unistd.h>

int flag = 0;

pthread_mutex_t lock;
pthread_cond_t cond;

void *func1(void *p)
{
    pthread_mutex_lock(&lock);
    while (!flag) {
        printf("T1 : Waiting for flag to become TRUE.\n");
        pthread_cond_wait(&cond, &lock); // This will put thread to sleep, and release the lock.
        sleep(1);
    }
    printf("T1 : Processing done.\n");
    pthread_mutex_unlock(&lock);
    return NULL;
}

void *func2(void *p)
{
    printf("T2 : Trying to take the lock.\n");
    pthread_mutex_lock(&lock);
    printf("T2 : Got the lock. Making flag as TRUE.\n");
    flag = 1;
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&cond); // I am done with the changes, send signal.
}

int main(void)
{
    pthread_t thread1, thread2;
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&thread1, NULL, func1, NULL);
    sleep(1); // Creating more chances of getting T1 into execution first, to showcase the issue of condition variables.
    pthread_create(&thread2, NULL, func2, NULL);


    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}
