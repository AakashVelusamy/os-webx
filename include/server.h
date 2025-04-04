#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;

void start_server();
void* handle_client(void* client_socket);
void handle_https_tunnel(int client_socket, const string& url);
void relay_data(int client_socket, int remote_socket);
string extract_host(const string& request);
int connect_to_remote_server(const string& hostname, int port);

#endif 
