#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_DOCS 20
#define MAX_PRINTERS 5

sem_t printer_available;
sem_t docuement_available;

int printer_id;
int document_id;
sem_t mutex; // mutex lock to protect writes to 'printer_id' and 'document_id'

/*
 * As this program is to just show the snchronization method between queues, I
 * am just keeping the queues small and of fixed sizes.
 * We can enhance this program further to make the queues more flexible.
 */
unsigned int printer_queue[MAX_PRINTERS] = {1, 2, 3, 4, 5};
unsigned int document_queue[MAX_DOCS] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                                         11, 12, 13, 14 ,15, 16, 17, 18, 19, 20};

void print(void *args)
{
    sem_wait(&mutex);
    printf("Thread %s : Writing Docuemtn%d to printer%d\n", (char *)args,
            document_queue[document_id], printer_queue[printer_id]);
    document_id = (document_id + 1) % MAX_DOCS;
    printer_id = (printer_id + 1) % MAX_PRINTERS;
    sleep(1);
    sem_post(&mutex);
    return;
}

void *printer_func(void *args)
{
    while(1) {
        sem_post(&printer_available); // Telling others that printer is avaiable.
        sem_wait(&docuement_available); // Waiting for document to be available.
        print(args);
    }
    return NULL;
}

void *document_func(void *args)
{
    while(1) {
        sem_post(&docuement_available); // Telling others that document is avaiable.
        sem_wait(&printer_available); // Waiting for printer to be available.
        print(args);
    }
    return NULL;
}

int main(void)
{
    pthread_t printer, document;
    printer_id = document_id = 0;

    sem_init(&printer_available, 0, 0);
    sem_init(&docuement_available, 0, 0);

    sem_init(&mutex, 0, 1); // binary semaphore for mutex lock.

    pthread_create(&printer, NULL, printer_func, "Printer");
    pthread_create(&document, NULL, document_func, "Document");

    pthread_join(printer, NULL);
    pthread_join(document, NULL);

    sem_destroy(&printer_available);
    sem_destroy(&docuement_available);
    sem_destroy(&mutex);

    return 0;
}
