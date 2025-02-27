#include "../include/definitions.h"
#include "../include/server.h"
#include <netdb.h>  // Required for gethostbyname()

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
    char buffer[8192] = {0};

    int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes_received <= 0) {
        cerr << "Failed to read request from client" << endl;
        close(sock);
        return NULL;
    }

    string request(buffer, bytes_received);
    cout << "Received Request:\n" << request << endl;

    string host = extract_host(request);
    if (host.empty()) {
        cerr << "Invalid HTTP request (no host found)" << endl;
        close(sock);
        return NULL;
    }

    int remote_server_fd = connect_to_remote_server(host, 80);
    if (remote_server_fd < 0) {
        cerr << "Failed to connect to remote server" << endl;
        close(sock);
        return NULL;
    }

    // Send the full request to the remote server
    send(remote_server_fd, request.c_str(), request.length(), 0);

    // Relay response back to client
    char response[8192];
    while ((bytes_received = recv(remote_server_fd, response, sizeof(response), 0)) > 0) {
        send(sock, response, bytes_received, 0);
    }

    close(remote_server_fd);
    close(sock);
    return NULL;
}

string extract_host(const string& request) {
    size_t start = request.find("Host: ");
    if (start == string::npos) return "";

    start += 6;
    size_t end = request.find("\r\n", start);
    return request.substr(start, end - start);
}

int connect_to_remote_server(const string& hostname, int port) {
    struct hostent* host = gethostbyname(hostname.c_str());
    if (!host) {
        cerr << "Failed to resolve hostname: " << hostname << endl;
        return -1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    memcpy(&server_addr.sin_addr, host->h_addr, host->h_length);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to remote server failed");
        close(sock);
        return -1;
    }

    return sock;
}

