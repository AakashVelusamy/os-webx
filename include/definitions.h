#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <pthread.h>
#include <semaphore.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <string>

using namespace std;

// Server settings
#define PORT 8080
#define BACKLOG 10
#define MAX_THREADS 5
#define CACHE_SIZE 5
#define BUFFER_SIZE 16384

// Thread pool variables
extern pthread_mutex_t lock;
extern sem_t semaphore;
extern queue<int> client_queue;

// Function declarations
void start_server();
void* handle_client(void* client_socket);
void initialize_thread_pool();
void* thread_function(void* arg);

// Cache function declarations
void initialize_cache();
string get_from_cache(string url);
void add_to_cache(string url, string content);

#endif

