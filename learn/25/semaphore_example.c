// Program to demonstrate Semaphore as a lock between two threads.

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
  
sem_t sem;
  
void *fun(void *arg)
{
    sem_wait(&sem);
    printf("%s : Entering the critical section.\n", (char *)arg);
    sleep(1);
    printf("%s : Leaving the critical section.\n", (char *)arg);
    sem_post(&sem);
}
  
int main()
{
    sem_init(&sem, 0, 1);
    pthread_t t1,t2;
    pthread_create(&t1, NULL, fun, "T1");
    pthread_create(&t2, NULL, fun, "T2");
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    sem_destroy(&sem);
    return 0;
}
