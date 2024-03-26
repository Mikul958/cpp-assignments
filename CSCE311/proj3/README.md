# Program 3 - Client/Server with Shared Memory

**server.cc / server.h:**  
Provides functionality for the server, including the ability to read in messages from the client and respond accordingly, as well as read in specified lines from a text file. Also serves as the entry point for the server.  
- Makes use of domain_socket.h to interact with client.  

**client.cc / client.h:**  
Provides functionality for the client, including the ability to interact with the server and evaluate the equations in lines received from server. Also serves as the entry point for the client.  
- Makes use of domain_socket.h to interact with server.
- Makes use of calculate.h to evaluate equations returned by the server.  

**domain_socket.cc / domain_socket.h:**  
Provided by CSCE311 GitHub repository. Provides functionality for the use of UNIX Domain Sockets, including the ability to start a server, connect clients, and pass messages between the two.  
- Modified to include the ability to build and parse messages in a standardized format so that the server and client do not have to determine their own format or build/parse message strings manually.  

**calculate.cc / calculate.h:**  
Included (and slightly modified) from Project 1. Called on by the client to evaluate equations obtained from the server after operands and operators are formatted as a vector by the client.