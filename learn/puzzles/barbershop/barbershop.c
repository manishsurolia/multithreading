#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int n;
int customers;

sem_t mutex;
sem_t customer;
sem_t barber;
sem_t customer_done;
sem_t barber_done;

void balk(void)
{
    while(1);
    return;
}

void get_hair_cut(char *customer_name)
{
    printf("%s : Getting hair cut done.\n", customer_name);
    return;
}

void *customer_func(void *args)
{
    sem_wait(&mutex);
    if (customers == n) {
        sem_post(&mutex);
        printf("%s Leaving. No chairs available.\n", (char *)args);
        balk();
    }
    customers++; //Scoreboared
    printf("%s taking chair.\n", (char *)args);
    sem_post(&mutex);

    sem_post(&customer); // Rendazvous1
    sem_wait(&barber);

    get_hair_cut(args);

    sem_post(&customer_done); // Rendazvous2
    sem_wait(&barber_done);

    sem_wait(&mutex);
    customers--; //Scoreboared
    sem_post(&mutex);

    return NULL;
}

void cut_hair(void)
{
    return;
}

void *barber_func(void *args)
{
    while (1) {
        sem_wait(&customer); // Rendazvous1
        sem_post(&barber);

        cut_hair();

        sem_wait(&customer_done); // Rendazvous2
        sem_post(&barber_done);
    }

    return NULL;
}

int main(void)
{
    pthread_t *customer_threads;
    int customers_count;
    pthread_t barber_thread;

    char customer_threads_name[150][100];

    customers = 0;
    sem_init(&mutex, 0, 1);
    sem_init(&barber, 0, 0);
    sem_init(&customer, 0, 0);
    sem_init(&barber_done, 0, 0);
    sem_init(&customer_done, 0, 0);

    printf("How many customers we want to send to the barber shop?");
    scanf("%d", &customers_count);

    for (int i=0; i<customers_count; i++) {
        sprintf(customer_threads_name[i], "Customer%d", i);
    }

    printf("How many chairs are present at barber shop?");
    scanf("%d", &n);

    customer_threads =
        (pthread_t *) malloc(sizeof(pthread_t) * customers_count);

    for (int i=0; i<customers_count; i++) {
        pthread_create(&customer_threads[i], NULL, customer_func,
                       customer_threads_name[i]);
    }
    pthread_create(&barber_thread, NULL, barber_func, NULL);

    for (int i=0; i<customers_count; i++) {
        pthread_join(customer_threads[i], NULL);
    }
    pthread_join(barber_thread, NULL);

    return 0;
}
