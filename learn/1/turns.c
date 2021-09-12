#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void my_turn()
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
    my_turn();
    your_turn();
    exit(0);
}
