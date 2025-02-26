#include "../include/definitions.h"

int main() {
    cout << "Starting WebX Proxy Server..." << endl;
    initialize_thread_pool();
    start_server();
    return 0;
}
