#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *my_turn(void *p)
{
    for (int i=0; i<10; i++) {
        sleep(1);
        printf("My Turn!! - %d\n", (i+1));
    }
}

void your_turn()
{
    for (int i=0; i<5; i++) {
        sleep(1);
        printf("Your Turn!! - %d\n", (i+1));
    }
}

int main()
{
    pthread_t t_id;
    pthread_create(&t_id, NULL, my_turn, NULL); // New thread executes this.
    your_turn(); // The main thread executs this.
    exit(0);
}
