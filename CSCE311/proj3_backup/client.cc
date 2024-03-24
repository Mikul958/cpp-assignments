// Copyright 2024 mpikula

#include <proj3/client.h>

double Client::EvaluateLine(string line) {
    vector<string> equation;

    string current;
    for (char c : line) {
        if (c == ' ') {
            equation.push_back(current);
            current.clear();
        } else {
            current += c;
        }
    }
    if (current.length() > 0)
        equation.push_back(current);

    // Remove "Line" and "<num>:" from equation vector
    equation.erase(equation.begin(), equation.begin()+2);

    return Calculate(equation);  // Using calculate.h from Project 1
}

void Client::Run(vector<string> request) {
    // Initalize client and attempt to connect to DomainSocket
    cout << "Client initializing..." << endl;
    if (!Init())
        exit(1);
    cout << "Client connecting..." << endl;
    if (!Connect())
        exit(2);
    cout << "SERVER CONNECTION ACCEPTED" << endl;

    // Create shared memory between this client and the server                                  TODO figure out shm


    // Build message string from request and write to server                                    TODO decide between shm and re-using domain sockets
    string message = BuildMessage(request);
    ::ssize_t bytes_written = Write(message);
    if (bytes_written < 0) {
        cerr << "DomainSocketClient terminating..." << endl;
        exit(3);
    } else if (bytes_written == 0) {
        cout << "Server disconnected" << endl;
        exit(4);
    }

    // Read in response from server and parse
    string response;
    ::ssize_t bytes_received = Read(&response);
    cout << "BYTES RECEIVED: " << bytes_received << endl;
    vector<string> returned = ParseMessage(response);

    // Check for error from server
    if (returned[0] == "0") {
        cout << returned[1] << endl;
        return;
    }

    // Parse response and print equations to console.
    for (string s : returned)
        cout << s << "  =  " << EvaluateLine(s) << endl;

    // Close shared memory                                                                      TODO figure out shm
}

int main(int argc, char* argv[]) {
    // Validate usage
    if (argc != 4) {
        cerr << "\n    Usage : " << argv[0]
             << " <socket name> <filepath> <number of lines in file>\n" << endl;
        exit(5);
    }

    // Obtain socket name and build request vector of path and lines
    char* socket_name = argv[1];
    vector<string> request;
    for (int i=2; i < argc; ++i)
        request.push_back(argv[i]);

    // Connect to server with given socket name and request
    Client client(socket_name);
    client.Run(request);

    return 0;
}
