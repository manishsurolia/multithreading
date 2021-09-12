#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *my_turn(void *p)
{
    while (1) {
        sleep(1);
        printf("My Turn!!\n");
    }
}

void your_turn()
{
    while (1) {
        sleep(1);
        printf("Your Turn!!\n");
    }
}

int main()
{
    pthread_t t_id;
    pthread_create(&t_id, NULL, my_turn, NULL); // New thread executes this.
    //my_turn();
    your_turn(); // The main thread executs this.
    exit(0);
}
