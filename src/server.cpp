#include "../include/definitions.h"
#include "../include/server.h"
#include "../include/cache.h"

#include <netdb.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <cstring>
#include <algorithm>

using namespace std;

// Start the proxy server
void start_server() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, BACKLOG) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    cout << "Server listening on port " << PORT << endl;

    while (true) {
        sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        // Add client to queue for thread pool
        pthread_mutex_lock(&lock);
        client_queue.push(client_socket);
        pthread_mutex_unlock(&lock);
        sem_post(&semaphore);
    }
}

// Handle client connection
void* handle_client(void* arg) {
    int client_socket = *((int*)arg);
    char buffer[BUFFER_SIZE] = {0};
    int n = read(client_socket, buffer, BUFFER_SIZE);
    if (n <= 0) {
        close(client_socket);
        return NULL;
    }

    string request(buffer);
    string method = request.substr(0, request.find(" "));
    string url;

    if (method == "GET") {
        size_t pos1 = request.find(" ") + 1;
        size_t pos2 = request.find(" ", pos1);
        url = request.substr(pos1, pos2 - pos1);

        string content;
        if (cache_get(url, content)) {
            cout << "[CACHE HIT] " << url << endl;
            write(client_socket, content.c_str(), content.size());
        } else {
            cout << "[CACHE MISS] " << url << endl;

            // Fetch from remote server
            string host = extract_host(request);
            int remote_socket = connect_to_remote_server(host, 80);
            if (remote_socket < 0) {
                close(client_socket);
                return NULL;
            }

            write(remote_socket, buffer, n);

            string response;
            char resp_buffer[BUFFER_SIZE];
            int bytes_read;
            while ((bytes_read = read(remote_socket, resp_buffer, BUFFER_SIZE)) > 0) {
                response.append(resp_buffer, bytes_read);
            }

            cache_put(url, response);
            write(client_socket, response.c_str(), response.size());
            close(remote_socket);
        }
    }
    close(client_socket);
    return NULL;
}

// Handle HTTPS CONNECT tunnel
void handle_https_tunnel(int client_socket, const string& host) {
    int remote_socket = connect_to_remote_server(host, 443);
    if (remote_socket < 0) {
        close(client_socket);
        return;
    }

    const char* response = "HTTP/1.1 200 Connection Established\r\n\r\n";
    write(client_socket, response, strlen(response));

    relay_data(client_socket, remote_socket);

    close(client_socket);
    close(remote_socket);
}

// Relay data between client and remote server
void relay_data(int client_socket, int remote_socket) {
    fd_set readfds;
    char buffer[BUFFER_SIZE];

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(client_socket, &readfds);
        FD_SET(remote_socket, &readfds);

        int maxfd = max(client_socket, remote_socket) + 1;
        int activity = select(maxfd, &readfds, nullptr, nullptr, nullptr);

        if (activity <= 0) break;

        if (FD_ISSET(client_socket, &readfds)) {
            int n = read(client_socket, buffer, BUFFER_SIZE);
            if (n <= 0) break;
            write(remote_socket, buffer, n);
        }

        if (FD_ISSET(remote_socket, &readfds)) {
            int n = read(remote_socket, buffer, BUFFER_SIZE);
            if (n <= 0) break;
            write(client_socket, buffer, n);
        }
    }
}

// Extract host from HTTP request
string extract_host(const string& request) {
    size_t host_pos = request.find("Host:");
    if (host_pos == string::npos) return "";

    size_t start = host_pos + 5;
    size_t end = request.find("\r\n", start);
    string host = request.substr(start, end - start);
    host.erase(remove(host.begin(), host.end(), ' '), host.end());
    return host;
}

// Connect to remote server
int connect_to_remote_server(const string& hostname, int port) {
    struct hostent* he = gethostbyname(hostname.c_str());
    if (!he) return -1;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return -1;

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = *((struct in_addr*)he->h_addr);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        close(sock);
        return -1;
    }

    return sock;
}

