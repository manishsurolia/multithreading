#include <stdio.h>
#include <pthread.h>

unsigned int counter = 0;

void *func(void *p)
{
    for (unsigned int i=0; i<1000000000; i++) {
        /*
         * Below line is Critical section of this function, as it is trying to
         * update the global variable.
         */
        counter++;
    }
    return NULL;
}

int main(void)
{
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, func, NULL);
    pthread_create(&thread2, NULL, func, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("counter = %d\n", counter);

    return 0;
}
