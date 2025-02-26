#include "../include/definitions.h"

void start_server() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, BACKLOG) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    cout << "Server listening on port " << PORT << "..." << endl;

    while (true) {
        if ((client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void*)&client_fd);
        pthread_detach(thread_id);
    }
}

void* handle_client(void* client_socket) {
    int sock = *(int*)client_socket;
    char buffer[1024] = {0};

    read(sock, buffer, 1024);
    cout << "Received: " << buffer << endl;

    string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello from WebX!";
    send(sock, response.c_str(), response.length(), 0);

    close(sock);
    return NULL;
}
