#ifndef SERVER_H
#define SERVER_H

#include "definitions.h"

// Function declarations
void start_server();
void* handle_client(void* client_socket);
string extract_host(const string& request);
int connect_to_remote_server(const string& hostname, int port);

#endif // SERVER_H

