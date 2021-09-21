#include <stdio.h>
#include <pthread.h>

unsigned int counter = 0;
unsigned int lock;

/*
 * Return only when you get the lock, otherwise keep waiting for the lock.
 */
void my_lock(int *p, char *str)
{
    /* Loop, until the lock is not free. */
    while(*p) {
        printf("%s : Waiting for the lock\n", str);
    }

    printf("%s : Acquired the lock\n", (char *) str);
    *p = 1;
}

/*
 * Make the lock free and come out.
 */
void my_unlock(int *p, char *str)
{
    *p = 0;
    printf("%s : Released the lock\n", str);
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
void *increment(void *p)
{
    for (unsigned int i = 0; i < 100000; i++) {
        my_lock(&lock, p);
        counter++;
        my_unlock(&lock, p);
    }
    return NULL;
}

int main(void)
{
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, increment, "T1");
    pthread_create(&thread2, NULL, increment, "T2");
    my_lock_init(&lock);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Final value of counter : %d\n", counter);

    return 0;
}
