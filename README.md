# WebX - Multithreaded Web Proxy

## Overview
WebX is a high-performance multithreaded web proxy that implements key operating system concepts such as **sockets, multithreading, process scheduling, thread synchronization, and memory management.** It is designed for efficiency and scalability while handling multiple client requests.

## Features (Updated ✅)
- ✅ **Basic client-server connection established using sockets.**  
- ✅ **Thread pool implemented** for handling multiple client requests efficiently.  
- ✅ **LRU caching mechanism** to store frequently accessed web pages.  
- ✅ **Finalized fixed project structure.**  
- ✅ **Client successfully connected and received response from server.**  
- ✅ **Future implementation: Parsing locally stored HTML files.**  

## Compilation & Execution (Updated ✅)
```sh
make
./bin/webx   # Run the server
./bin/client # Run the client
```

## Simple Test (Updated ✅)
```
Starting WebX Proxy Server... <br>
Server listening on port 8080... <br>
Received: Hello from client! <br>

Connected to server! <br>
Server response: HTTP/1.1 200 OK <br>
Content-Type: text/plain <br>

Hello from WebX! 
```
