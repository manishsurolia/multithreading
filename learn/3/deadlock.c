#include <stdio.h>
#include <pthread.h>

void* func(void* tid)
{
    printf("Waiting for %lu for finish.\n", *(unsigned long *)tid);
    pthread_join((*((pthread_t*)tid)), NULL);
    return NULL;
}

int main(void)
{
    pthread_t thread1 = pthread_self(); // Obtain ID of the calling thread, main thread.
    pthread_t thread2;
    pthread_create(&thread2, NULL, func, &thread1);
    func(&thread2);
    return 0;
}
