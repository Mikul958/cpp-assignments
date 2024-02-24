// Copyright 2024 mpikula

#include <proj2/client.h>

#include <vector>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;

using std::string;

void Client::Run() {
    cout << "Client initializing..." << endl;
    if (!Init())
        exit(1);

    cout << "Client connecting..." << endl;
    if (!Connect())
        exit(2);
    cout << "SERVER CONNECTION ACCEPTED";

    while (true) {
        string input;
        std::getline(cin, input);
        ::ssize_t bytes_wrote = Write(input);
        if (bytes_wrote < 0) {
            cerr << "DomainSocketClient terminating..." << endl;
            exit(3);
        }
        else if (bytes_wrote == 0) {
            cout << "Server disconnected" << endl;
            exit(4);
        }
    }
}

int main(int argc, char* argv[]) {
    // Validate usage
    if (argc < 4) {
        cerr << " Usage : " << argv[0]
             << "<server name> <filepath> <line 0> ... <line n>" << endl;
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
    client.Run();

    exit(0);
}