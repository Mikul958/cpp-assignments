# Program 3 - Client/Server with Shared Memory

**server.cc / server.h:**  
Provides functionality for the server:
- Creates named semaphores for the server and client, and destroys them upon termination.
- Opens shared memory created by the client:
    - Reads in file path and line numbers to from shared memory and reads specified file.
    - Responds to client with file lines, or file read errors if any.

Includes:
- shared_mem.h

**client.cc / client.h:**  
Provides functionality for the client:
- Creates a shared memory location for the server and client, and destroys it upon completion.
    - Sends file path and line count through shared memory.
    - Receives error status through shared memory, and file lines through main shared memory buffer.
- Synchronizes using named semaphores created by the server.
- Tokenizes lines from server and calculates them using functions from calculate.h.
    - Creates 4 threads to quarter amount of shared memory evaluated and speed up the process.

Includes:
- shared_mem.h
- calculate.h

**shared_mem.h:**  
Contains structure of shared memory location, as well as hard-coded values related to shared memory and semaphores shared by the client and server.

**calculate.cc / calculate.h:**  
Included (and slightly modified) from Project 1. Used by the client to evaluate the equations located in main shared memory buffer.