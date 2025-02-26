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

using namespace std;

// Server Config
#define PORT 8080
#define BACKLOG 10
#define MAX_THREADS 5
#define CACHE_SIZE 5

// Function Prototypes
void start_server();
void* handle_client(void* client_socket);
void initialize_thread_pool();
void* thread_function(void* arg);
void initialize_cache();
string get_from_cache(string url);
void add_to_cache(string url, string content);

#endif
