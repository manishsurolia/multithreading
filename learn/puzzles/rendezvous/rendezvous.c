#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t a1done, b1done;

void * thread_a_func(void *args)
{
    printf("statement a1\n");
    sem_post(&a1done);
    sem_wait(&b1done);
    printf("statement a2\n");
}

void * thread_b_func(void *args)
{
    printf("statement b1\n");
    sem_post(&b1done);
    sem_wait(&a1done);
    printf("statement b2\n");
}

int main(void)
{
    pthread_t thread_a, thread_b;
    sem_init(&a1done, 0 ,0);
    sem_init(&b1done, 0 ,0);

    pthread_create(&thread_a, NULL, thread_a_func, NULL);
    pthread_create(&thread_b, NULL, thread_b_func, NULL);

    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);

    return 0;
}
