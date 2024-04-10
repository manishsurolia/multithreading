#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_key_t key;
pthread_once_t once_control = PTHREAD_ONCE_INIT;

// Destructor function for thread-specific data
void destructor(void *value) {
    printf("Thread id : %lu, Destructor called, value = %p\n", pthread_self(), value);
    free(value); // Free the memory allocated for the thread-specific data
}

// Initialize the key once
void initialize_key() {
    pthread_key_create(&key, destructor);
}

// Thread function
void *thread_function(void *arg) {
    pthread_once(&once_control, initialize_key); // Ensure key is initialized only once

    int *thread_data = (int *)malloc(sizeof(int));
    *thread_data = *((int *)arg); // Copy the argument to thread-specific data
    printf("Thread id : %lu, Setting up Thread-specific data in the key: %p\n", pthread_self(), thread_data);
    pthread_setspecific(key, thread_data); // Set thread-specific data
    printf("Thread id : %lu, Getting thread-specific data from the key: %p\n", pthread_self(), pthread_getspecific(key));

    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    int data1 = 10, data2 = 20;

    // Create threads
    pthread_create(&thread1, NULL, thread_function, &data1);
    pthread_create(&thread2, NULL, thread_function, &data2);

    // Wait for threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_key_delete(key); // Delete the thread-specific data key

    return 0;
}
