// Copyright 2024 mpikula

#include <proj2/client.h>

void Client::Run(string message) {
    // Initalize client and attempt to connect to DomainSocket
    cout << "Client initializing..." << endl;
    if (!Init())
        exit(1);
    cout << "Client connecting..." << endl;
    if (!Connect())
        exit(2);
    cout << "SERVER CONNECTION ACCEPTED\n";



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

    // Write request to server and print bytes written
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
    string received;
    ::ssize_t bytes_received = Read(&received);
    cout << "BYTES RECEIVED: " << bytes_received << endl;

    
    
    // Test code, checking response from server.                            TODO delete when finished
    cout << "      TEST - RECEIVED MESSAGE: ";
    for (char c : received) {
        if (c == '\037')
            cout << "|US|";
        else if (c == '\004')
            cout << "|EOT|";
        else if (c == '\r')
            cout << "|R|";
        else
            cout << c;
    }
    cout << endl;
}

int main(int argc, char* argv[]) {
    // Validate usage
    if (argc < 4) {
        cerr << "\n    Usage : " << argv[0]
             << " <socket name> <filepath> <line 0> ... <line n>\n" << endl;
        exit(5);
    }
    char* socket_name = argv[1];

    // Using default US and EoT chars from DomainSocket class
    const char kUS = DomainSocket::kUS;
    const char kEoT = DomainSocket::kEoT;

    // Build message string
    string message = string(argv[2]);
    for (int i=3; i < argc; ++i)
        message = (message + kUS) + argv[i];
    message += kEoT;

    // Connect to server with given socket name.
    Client client(socket_name);
    client.Run(message);

    exit(0);
}
