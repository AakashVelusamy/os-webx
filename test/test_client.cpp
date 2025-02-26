#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        cerr << "Error creating socket.\n";
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);  // Must match server's port
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        cerr << "Connection failed.\n";
        close(sock);
        return 1;
    }

    cout << "Connected to server!\n";

    // Send a test message
    const char* message = "Hello from client!";
    send(sock, message, strlen(message), 0);

    // Receive response (optional)
    char buffer[1024] = {0};
    recv(sock, buffer, sizeof(buffer), 0);
    cout << "Server response: " << buffer << endl;

    close(sock);
    return 0;
}
