// Copyright 2024 mpikula

#include <proj2/client.h>

void Client::Run(vector<string> request) {
    // Initalize client and attempt to connect to DomainSocket
    cout << "Client initializing..." << endl;
    if (!Init())
        exit(1);
    cout << "Client connecting..." << endl;
    if (!Connect())
        exit(2);
    cout << "SERVER CONNECTION ACCEPTED" << endl;

    // Build message string from request and write to server
    string message = BuildMessage(request);
    ::ssize_t bytes_written = Write(message);
    if (bytes_written < 0) {
        cerr << "DomainSocketClient terminating..." << endl;
        exit(3);
    }
    else if (bytes_written == 0) {
        cout << "Server disconnected" << endl;
        exit(4);
    }
    cout << "BYTES WRITTEN: " << bytes_written << endl;

    // Read in response from server
    string response;
    ::ssize_t bytes_received = Read(&response);
    cout << "BYTES RECEIVED: " << bytes_received << endl;
    vector<string> returned = ParseMessage(response);

    /*
    // Test code                                                        TODO delete when finished
    cout << "      TEST - SENDING MESSAGE: ";
    for (char c : message) {
        if (c == '\037')
            cout << "|US|";
        else if (c == '\004')
            cout << "|EOT|";
        else
            cout << c;
    }
    */

    // test code                                                TODO delete when finished
    for (string s : returned)
        cout << s << endl;
    
    // TODO parse responses from server and evaluate using calculate.cc
}

int main(int argc, char* argv[]) {
    // Validate usage
    if (argc < 4) {
        cerr << "\n    Usage : " << argv[0]
             << " <socket name> <filepath> <line 0> ... <line n>\n" << endl;
        exit(5);
    }

    // Obtain socket name and build request vector of path and lines
    char* socket_name = argv[1];
    vector<string> request;
    for (int i=2; i<argc; ++i)
        request.push_back(argv[i]);

    // Connect to server with given socket name and request
    Client client(socket_name);
    client.Run(request);

    exit(0);
}
