#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int customer_count;
int chair_count;

sem_t mutex;
sem_t barber_sem;

void *customer_func(void *args)
{
    sleep(rand()%10);
    if (customer_count < chair_count) {
        sem_wait(&mutex);
        customer_count++;
        printf("Customer taking a seat.\n");
        sem_post(&barber_sem);
        sem_post(&mutex);
    } else {
        printf("All chairs are full. Customer returning.\n");
    }

    return NULL;
}

void hair_cut(void)
{
    printf("Barber doing haircut.\n");
    return;
}

void *barber_func(void *args)
{
    while (1) {
        sem_wait(&barber_sem);
        sem_wait(&mutex);
        customer_count--;
        sem_post(&mutex);
        hair_cut();
    }
    return NULL;
}

int main(void)
{
    pthread_t *customer_threads;
    int customers_count;
    pthread_t barber;

    customer_count = 0;
    sem_init(&mutex, 0, 1);
    sem_init(&barber_sem, 0, 0);

    printf("How many customers we want to send?");
    scanf("%d", &customers_count);

    printf("How many chairs are present?");
    scanf("%d", &chair_count);

    customer_threads =
        (pthread_t *) malloc(sizeof(pthread_t) * customers_count);

    for (int i=0; i<customers_count; i++) {
        pthread_create(&customer_threads[i], NULL, customer_func, NULL); 
    }
    pthread_create(&barber, NULL, barber_func, NULL);

    for (int i=0; i<customers_count; i++) {
        pthread_join(customer_threads[i], NULL); 
    }
    pthread_join(barber, NULL);

    return 0;
}
