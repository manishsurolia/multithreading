#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void * func(void *var)
{
    sleep(2);
    printf("\nWe are in the created thread");
    return NULL;
}

int main()
{
    pthread_t t_id;
    printf("\nBefore creating thread");
    pthread_create(&t_id, NULL, func, NULL);
    pthread_join(t_id, NULL);
    printf("\nAfter thread\n");
    exit(0);
} 
