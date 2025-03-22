# WebX

## Developed by
- 23PT01 - Aakash Velusamy  
- 23PT37 - Vaishnavi V  

## Overview

WebX is a web proxy server built in C++ that combines important concepts like multithreading, synchronization, and socket programming to build a proxy that manages HTTP and HTTPS requests with ease. We put a lot of thought into making it handle multiple tasks at once and speed things up with a thread pool and caching, showing how operating systems work under the hood. WebX acts as a go-between for a client (like your browser) and web servers, picking up requests, sending them off, and delivering the replies.

---

## Key Features

| Feature                  | Description                                                                                     |
|--------------------------|-------------------------------------------------------------------------------------------------|
| **Multithreading**       | Runs multiple threads at once to handle client requests without delay.                         |
| **Thread Pool Implementation**          | Keeps a set number of threads ready to pick up and process client connections.                  |
| **Thread Synchronization** | Uses tools like mutexes and semaphores to keep threads working together safely.                |
| **Cache Management**     | Saves recent content using a Least Recently Used (LRU) system—when full, it drops the oldest entry to make room for new ones, tracked with a map and list. |
| **Socket Programming**   | Handles network communication with sockets to connect clients and servers smoothly.             |
| **HTTP Request Handling**| Takes care of HTTP requests (like GET or POST) and HTTPS tunnels, passing them along correctly.|

---

### • Multithreading
| Aspect                  | Explanation                                                                                              |
|-------------------------|----------------------------------------------------------------------------------------------------------|
| **What It Is**          | Running multiple threads at the same time to tackle different tasks, like handling client requests.      |
| **Why It Matters**      | Lets the proxy manage many clients at once without slowing down, making it more responsive.             |
| **How I Used It**       | Set up threads to take on client sockets as they come in, letting the server juggle multiple jobs.       |
| **OS Tie-In**           | Shows how the OS switches between threads and keeps them running smoothly together.                    |

Multithreading is the heart of WebX’s ability to stay quick. Each client gets a thread to handle its request, so no one’s waiting longer than they need to.

---

### • Thread Pool Implementation
| Aspect                  | Explanation                                                                                              |
|-------------------------|----------------------------------------------------------------------------------------------------------|
| **What It Is**          | A collection of pre-allocated threads that execute concurrently to process client requests efficiently.  |
| **Why It Matters**      | Mitigates the overhead associated with creating threads dynamically, enhancing scalability and resource utilization. |
| **How I Used It**       | Configured a fixed number of worker threads (five by default) to grab client sockets from a shared queue.|
| **OS Tie-In**           | Reflects the role of the OS in thread scheduling, context switching, and resource allocation.                |

The thread pool means we don’t start fresh threads every time - a group of five are always ready. They pick up sockets from a queue, keeping the CPU busy but under control.

---

### • Thread Synchronization
| Aspect                  | Explanation                                                                                              |
|-------------------------|----------------------------------------------------------------------------------------------------------|
| **What It Is**          | Mechanisms such as mutexes and semaphores that regulate access to shared resources among threads.       |
| **Why It Matters**      | Ensures data integrity and prevents race conditions in a multithreaded environment.                    |
| **How I Used It**       | Used mutexes to lock the client queue and semaphores to cap how many threads run at once.               |
| **OS Tie-In**           | Emulates the use of synchronization tools to coordinate processes and threads effectively.         |

Synchronization keeps our threads in line. Mutexes make sure only one thread touches the queue at a time, and semaphores stop too many from running, so everything stays safe and steady.

---

### • Cache Management
| Aspect                  | Explanation                                                                                              |
|-------------------------|----------------------------------------------------------------------------------------------------------|
| **What It Is**          | A system that stores recently accessed content, evicting the least recently used items to optimize memory usage. |
| **Why It Matters**      | Reduces latency by delivering cached responses, minimizing redundant network requests.                |
| **How I Used It**       | Built an LRU cache with an unordered map for quick lookups and a vector to track order, holding up to five items - when full, the oldest gets removed. |
| **OS Tie-In**           | Parallels OS memory management strategies, like page replacement algorithms.                          |

The LRU cache saves the last five responses we’ve seen. The unordered map lets us find them fast, and the vector keeps track of what’s old, kicking it out when we hit the limit. It’s a smart way to cut down on web calls.

---

### • Socket Programming
| Aspect                  | Explanation                                                                                              |
|-------------------------|----------------------------------------------------------------------------------------------------------|
| **What It Is**          | A framework for network communication using sockets to connect clients, proxies, and remote servers.   |
| **Why It Matters**      | Enables the proxy to serve as an intermediary, facilitating seamless data exchange over networks.      |
| **How I Used It**       | Used TCP sockets to link clients to the proxy and the proxy to servers, passing data back and forth.   |
| **OS Tie-In**           | Depends on the OS network stack and file descriptor management for smooth communication.             |

Sockets let WebX connect everything. TCP sockets handle the back-and-forth—whether it’s regular HTTP or HTTPS tunnels - while the OS keeps the data moving without us worrying about the details.

---

### • HTTP Request Handling
| Aspect                  | Explanation                                                                                              |
|-------------------------|----------------------------------------------------------------------------------------------------------|
| **What It Is**          | The process of managing HTTP requests (like GET or POST) and HTTPS tunneling for clients.              |
| **Why It Matters**      | Makes sure the proxy can handle all kinds of web requests properly, from simple pages to secure sites. |
| **How I Used It**       | Wrote code to spot request types, forward HTTP ones to servers, and set up HTTPS tunnels with CONNECT.  |
| **OS Tie-In**           | Ties into how the OS manages I/O and network tasks to keep data flowing between client and server.     |

HTTP handling is where WebX shines. It reads requests, sends GETs and POSTs to the right place, and for HTTPS, it opens a tunnel with CONNECT, passing encrypted data both ways.

---

## Project Workflow

1. **Server Initialization**: The proxy starts listening on port 8080 (our default) for clients.
2. **Client Management**: New connections get picked up by threads waiting in the pool.
3. **Request Handling**:
   - **HTTP**: Sends requests to servers and brings back replies for clients.
   - **HTTPS**: Sets up secure tunnels with CONNECT and passes encrypted data both ways.
4. **Caching**: Checks if we’ve got the response saved; if not, grabs it, stores it, and sends it out.
5. **Resource Release**: Closes sockets and frees up space after each request.

This flow pulls all our OS ideas together into something that works.

---

## Execution 
### Server Terminal
```
./bin/webx   
```
### Client Terminal
```
curl -x http://localhost:8080 http://example.com
```

### Server Output
```
Starting WebX Proxy Server...
Server listening on port 8080...
Received Request:
GET http://example.com/ HTTP/1.1
Host: example.com
User-Agent: curl/8.5.0
Accept: */*
Proxy-Connection: Keep-Alive
```
### Client Output
```
<!doctype html>
<html>
<head>
    <title>Example Domain</title>
    <meta charset="utf-8" />
    <meta http-equiv="Content-type" content="text/html; charset=utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    ...
</head>
</html>
```
---
## Conclusion

WebX is more than just a proxy - it’s our way of showing how operating system ideas play out in real life. We took multithreading, synchronization, sockets, cache management, and resource management from our coursework and turned them into a working server that handles web traffic. The thread pool and cache make it quick, while sockets and synchronization keep it steady when things get busy. We learned that these aren’t just notes - they are tools that help us to build something that really helps out.

---

*Developed for 23XT44 - Operating Systems - IV Semester*
