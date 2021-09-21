#include <stdio.h>
#include <pthread.h>

unsigned long int counter = 0;
unsigned int lock;

/*
 * Return only when you get the lock, otherwise keep waiting for the lock.
 */
void my_lock(int *p)
{
    /* Loop, until the lock is not free. */
    while(__sync_val_compare_and_swap (p, 0, 1) != 0);
    return;
}

/*
 * Make the lock free and come out.
 */
void my_unlock(int *p)
{
    *p = 0;
}

/*
 * Initialize the lock.
 */
void my_lock_init(int *p)
{
    *p = 0;
}

/*
 * Each thread executes this function. acquire and relase lock before executing
 * Critical section code.
 */
void *increment(void *arg)
{
    for (unsigned long int i = 0; i < 100000000; i++) {
        my_lock(&lock);
        counter++;
        my_unlock(&lock);
    }
    return NULL;
}

int main(void)
{
    pthread_t thread1, thread2, thread3, thread4;
    pthread_create(&thread1, NULL, increment, NULL);
    pthread_create(&thread2, NULL, increment, NULL);
    pthread_create(&thread3, NULL, increment, NULL);
    pthread_create(&thread4, NULL, increment, NULL);
    my_lock_init(&lock);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);

    printf("Final value of counter : %ld\n", counter);

    return 0;
}
