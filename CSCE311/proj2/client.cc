// Copyright 2024 mpikula

#include <proj2/client.h>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;

using std::string;
using std::vector;

void Client::Run(string message) {
    cout << "Client initializing..." << endl;
    if (!Init())
        exit(1);

    cout << "Client connecting..." << endl;
    if (!Connect())
        exit(2);
    cout << "SERVER CONNECTION ACCEPTED\n";

    cout << "SENDING MESSAGE: ";
    for (char c : message) {
        if (c == '\037')
            cout << "|US|";
        else if (c == '\004')
            cout << "|EOT|";
        else
            cout << c;
    }

    // TODO this possibly segfaults, keep an eye on it.
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

    /*
    string received;
    ::ssize_t bytes_received = Read(&received);

    cout << "BYTES_RECEIVED: " << bytes_received << endl;
    cout << "MESSAGE: " << received << endl;
    */

    /*
    while (true) {
        cout << "SENDING MESSAGE: ";
        for (char c : message) {
            if (c == '\037')
                cout << "|US|";
            else if (c == '\004')
                cout << "|EOT|";
            else
                cout << c;
        }
        
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
    }
    */
}

int main(int argc, char* argv[]) {
    // Validate usage
    if (argc < 4) {
        cerr << " Usage : " << argv[0]
             << " <socket name> <filepath> <line 0> ... <line n>" << endl;
        exit(5);
    }
    char* socket_name = argv[1];

    // Using default US and EoT chars from DomainSocket class
    char us = DomainSocket::kUS;
    char eot = DomainSocket::kEoT;

    // Build message string
    string message = string(argv[2]);
    for (int i=3; i < argc; ++i)
        message = (message + us) + argv[i];
    message += eot;

    message = us + (eot + message);  // Prepend US and EoT chars

    // Connect to server with given socket name.
    Client client(socket_name);
    client.Run(message);

    exit(0);
}
