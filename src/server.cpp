#include <sstream>  
#include "../include/definitions.h"
#include "../include/server.h"
#include <netdb.h>  
#include <fcntl.h>

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

    cout << "WebX Proxy Server listening on port " << PORT << "..." << endl;

    while (true) {
        if ((client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }

        pthread_t thread_id;
        int* client_sock_ptr = new int(client_fd);
        pthread_create(&thread_id, NULL, handle_client, (void*)client_sock_ptr);
        pthread_detach(thread_id);
    }
}

void* handle_client(void* client_socket) {
    int sock = *(int*)client_socket;
    delete (int*)client_socket; 
    char buffer[8192] = {0};

    int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes_received <= 0) {
        cerr << "Failed to read request from client" << endl;
        close(sock);
        return NULL;
    }

    string request(buffer, bytes_received);
    cout << "Received Request:\n" << request << endl;

    istringstream request_stream(request);
    string method, url, http_version;
    request_stream >> method >> url >> http_version;

    if (method == "CONNECT") {
        handle_https_tunnel(sock, url);
        return NULL;
    }

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

    send(remote_server_fd, request.c_str(), request.length(), 0);

    char response[8192];
    while ((bytes_received = recv(remote_server_fd, response, sizeof(response), 0)) > 0) {
        send(sock, response, bytes_received, 0);
    }

    close(remote_server_fd);
    close(sock);
    return NULL;
}

void handle_https_tunnel(int client_socket, const string& url) {
    size_t colon_pos = url.find(':');
    string hostname = url.substr(0, colon_pos);
    int port = stoi(url.substr(colon_pos + 1));

    int remote_socket = connect_to_remote_server(hostname, port);
    if (remote_socket < 0) {
        cerr << "Failed to connect to " << hostname << endl;
        close(client_socket);
        return;
    }

    string response = "HTTP/1.1 200 Connection Established\r\n\r\n";
    send(client_socket, response.c_str(), response.size(), 0);

    relay_data(client_socket, remote_socket);

    close(client_socket);
    close(remote_socket);
}

void relay_data(int client_socket, int remote_socket) {
    char buffer[8192];
    fd_set read_fds;
    int max_fd = max(client_socket, remote_socket) + 1;

    while (true) {
        FD_ZERO(&read_fds);
        FD_SET(client_socket, &read_fds);
        FD_SET(remote_socket, &read_fds);

        if (select(max_fd, &read_fds, NULL, NULL, NULL) < 0) {
            cerr << "select() failed\n";
            break;
        }

        if (FD_ISSET(client_socket, &read_fds)) {
            int bytes = recv(client_socket, buffer, sizeof(buffer), 0);
            if (bytes <= 0) break;
            send(remote_socket, buffer, bytes, 0);
        }

        if (FD_ISSET(remote_socket, &read_fds)) {
            int bytes = recv(remote_socket, buffer, sizeof(buffer), 0);
            if (bytes <= 0) break;
            send(client_socket, buffer, bytes, 0);
        }
    }
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
