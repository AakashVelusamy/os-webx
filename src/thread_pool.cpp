#include "../include/definitions.h"

pthread_mutex_t lock;
sem_t semaphore;
queue<int> client_queue;

void initialize_thread_pool() {
    pthread_mutex_init(&lock, NULL);
    sem_init(&semaphore, 0, MAX_THREADS);

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_t thread;
        pthread_create(&thread, NULL, thread_function, NULL);
        pthread_detach(thread);
    }
}

void* thread_function(void* arg) {
    while (true) {
        sem_wait(&semaphore);
        pthread_mutex_lock(&lock);

        if (!client_queue.empty()) {
            int client_socket = client_queue.front();
            client_queue.pop();

            pthread_mutex_unlock(&lock);
            handle_client((void*)&client_socket);
        } else {
            pthread_mutex_unlock(&lock);
        }
    }
    return NULL;
}
