#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

unsigned char flag;

void *func1(void *arg)
{
    while (!flag) {
        printf("T1 : Waiting for flag....\n");
        sleep(1); // Lets wait for 1 second before trying again.
    }
    printf("T1 : flag is TRUE. Exiting\n");
    return NULL;
}

void * func2(void *arg)
{
    // Let me wait for 10 seconds before I make the flag 1 (TRUE).
    sleep(10);
    flag = 1;
    return NULL;
}

int main(void)
{
    pthread_t thread1, thread2;
    flag = 0; // Initializing the flag with 0 (FALSE).

    pthread_create(&thread1, NULL, func1, NULL);
    pthread_create(&thread2, NULL, func2, NULL);
    
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
