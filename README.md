# WebX - Multithreaded Web Proxy

### Overview
WebX is a high-performance multithreaded web proxy that implements key operating system concepts such as **sockets, multithreading, process scheduling, thread synchronization, and memory management.** It is designed for efficiency and scalability while handling multiple client requests.

### Features (Updated ✅)
- ✅ **Finalized fixed project structure.**  
- ✅ **Basic client-server connection established using sockets.**  
- ✅ **Thread pool implemented** for handling multiple client requests efficiently.  
- ✅ **LRU caching mechanism** to store frequently accessed web pages.  
- ✅ **Proxy server now supports real websites!**  
- ✅ **Successfully handles HTTP requests and forwards them to the intended destination.**  
- ✅ **Correctly parses and retrieves web pages from external servers.**  

### Compilation & Execution (Updated ✅)
```sh
make
./bin/webx   # Run the server
./bin/client # Run the client
```
### Simple Test (Updated ✅)
```
# Start the server
./bin/webx

# Test with cURL as a client
curl -x http://localhost:8080 http://example.com
```
#### Server Output
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
#### Client Output
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
### Next Steps

- 🔜 Enhance caching for better performance.
- 🔜 Implement support for HTTPS requests (CONNECT method).
- 🔜 Improve logging and debugging features.
