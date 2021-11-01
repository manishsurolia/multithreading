#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

#define PAPER 0x1
#define MATCHES 0x2
#define TOBACCO 0x4

int flag;

//resoruces
sem_t paper;
sem_t tobacco;
sem_t matches;

sem_t mutex;

// agent semaphore, to be used by 3 different agent threads.
sem_t agent_sem;

void *smoker1_func(void *args)
{
    while (1) {
        // Has papers.
        while (!((flag & TOBACCO) && (flag & MATCHES)));
        printf("Smoker1 : Got tobacco and matches\n");

        // Will make and consume the ciggaretes.

        flag = 0;
        sem_post(&agent_sem);
    }
    return NULL;
}

void *smoker2_func(void *args)
{
    while (1) {
        // Has tobacco
        while (!((flag & PAPER) && (flag & MATCHES)));
        printf("Smoker2 : Got paper and matches\n");

        // Will make and consume the ciggaretes.

        flag = 0;
        sem_post(&agent_sem);
    }
    return NULL;
}

void *smoker3_func(void *args)
{
    while (1) {
        // Has matches
        while (!((flag & PAPER) && (flag & TOBACCO)));
        printf("Smoker3 : Got paper and tobacco\n");

        // Will make and consume the ciggaretes.

        flag = 0;
        sem_post(&agent_sem);
    }
    return NULL;
}

void *agent1_func(void *args)
{
    while (1) {
        sem_wait(&agent_sem);
        printf("Agent1 adding paper and tobacco\n");
        sem_post(&paper);
        sem_post(&tobacco);
    }
    return NULL;
}

void *agent2_func(void *args)
{
    while (1) {
        sem_wait(&agent_sem);
        printf("Agent2 adding paper and matches\n");
        sem_post(&paper);
        sem_post(&matches);
    }
    return NULL;
}

void *agent3_func(void *args)
{
    while (1) {
        sem_wait(&agent_sem);
        printf("Agent3 adding tobacco and matches\n");
        sem_post(&tobacco);
        sem_post(&matches);
    }
    return NULL;
}

void *get_paper_func(void *args)
{
    while (1) {
        sem_wait(&paper);
        sem_wait(&mutex);
        flag |= PAPER;
        sem_post(&mutex);
    }
    return NULL;
}

void *get_matches_func(void *args)
{
    while (1) {
        sem_wait(&matches);
        sem_wait(&mutex);
        flag |= MATCHES;
        sem_post(&mutex);
    }
    return NULL;
}

void *get_tobacco_func(void *args)
{
    while (1) {
        sem_wait(&tobacco);
        sem_wait(&mutex);
        flag |= TOBACCO;
        sem_post(&mutex);
    }
    return NULL;
}

int main(void)
{
    /*
     * There is actually only one agent, but we are making three agent threads
     * to make 3 different combinations of supply.
     *
     * Agent1 Thread: will supply paper and tobacco.
     * Agent2 Thread: Will supply paper and matches.
     * Agent3 Thread: Will supply tobacco and matches.
     *
     * Each of these three agent threads 'sem_wait' for a semaphore 'agent_sem',
     * supply two ingredients and then do 'sem_post', so that other agent thread
     * can take it.
     */
    pthread_t agent1, agent2, agent3;
    pthread_t smoker1, smoker2, smoker3;
    pthread_t paper_thread, tobacco_thread, matches_thread;
    sem_init(&paper, 0, 0);
    sem_init(&tobacco, 0, 0);
    sem_init(&matches, 0, 0);
    sem_init(&agent_sem, 0, 1);
    sem_init(&mutex, 0, 1);

    flag = 0;

    pthread_create(&agent1, NULL, agent1_func, NULL);
    pthread_create(&agent2, NULL, agent2_func, NULL);
    pthread_create(&agent3, NULL, agent3_func, NULL);

    pthread_create(&smoker1, NULL, smoker1_func, NULL);
    pthread_create(&smoker2, NULL, smoker2_func, NULL);
    pthread_create(&smoker3, NULL, smoker3_func, NULL);


    pthread_create(&paper_thread, NULL, get_paper_func, NULL);
    pthread_create(&tobacco_thread, NULL, get_tobacco_func, NULL);
    pthread_create(&matches_thread, NULL, get_matches_func, NULL);

    pthread_join(agent1, NULL);
    pthread_join(agent2, NULL);
    pthread_join(agent3, NULL);

    pthread_join(smoker1, NULL);
    pthread_join(smoker2, NULL);
    pthread_join(smoker3, NULL);

    pthread_join(paper_thread, NULL);
    pthread_join(tobacco_thread, NULL);
    pthread_join(matches_thread, NULL);

    return 0;
}
