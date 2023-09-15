#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_LEADERS 10
#define MAX_FOLLOWERS 10

sem_t leader_on_stage;
sem_t follower_on_stage;
sem_t dancer_mutex;

void * leader (void *args)
{
	sem_wait(&dancer_mutex);
	sem_post(&leader_on_stage);
	sem_wait(&follower_on_stage);
	printf("Leader %s goes to the stage.\n", (char *)args);
	sleep(1);
	sem_post(&dancer_mutex);
	return NULL;
}

void * follower (void *args)
{
	sem_post(&follower_on_stage);
	sem_wait(&leader_on_stage);
	printf("Follower %s goes to the stage.\n", (char *)args);
	sleep(1);
	return NULL;
}

int main(void)
{
	pthread_t leader1, leader2, leader3, leader4 , leader5;
	pthread_t follower1, follower2, follower3, follower4, follower5;

	sem_init(&leader_on_stage, 0, 0);
	sem_init(&follower_on_stage, 0, 0);
	sem_init(&dancer_mutex, 0, 1);

	pthread_create(&leader1, NULL, leader, "L1");
	pthread_create(&leader2, NULL, leader, "L2");
	pthread_create(&leader3, NULL, leader, "L3");
	pthread_create(&leader4, NULL, leader, "L4");
	pthread_create(&leader5, NULL, leader, "L5");


	pthread_create(&follower1, NULL, follower, "F1");
	pthread_create(&follower2, NULL, follower, "F2");
	pthread_create(&follower3, NULL, follower, "F3");
	pthread_create(&follower4, NULL, follower, "F4");
	pthread_create(&follower5, NULL, follower, "F5");


	pthread_join(leader1, NULL);
	pthread_join(leader2, NULL);
	pthread_join(leader3, NULL);
	pthread_join(leader4, NULL);
	pthread_join(leader5, NULL);

	pthread_join(follower1, NULL);
	pthread_join(follower2, NULL);
	pthread_join(follower3, NULL);
	pthread_join(follower4, NULL);
	pthread_join(follower5, NULL);

	return 0;
	
}
