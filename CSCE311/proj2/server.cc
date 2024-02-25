// Copyright 2024 mpikula

#include <proj2/server.h>

using std::cout;
using std::cerr;
using std::endl;

using std::string;
using std::vector;

vector<string> Server::Explode(string input, char us, char eot) {
    vector<string> args;
    string current;

    for (int i=0; input[i] != eot; i++) {
        if (input[i] == us) {
            args.push_back(current);
            current.clear();
        } else {
            current += input[i];
        }
    }
    if (current.size() > 0)
        args.push_back(current);

    return args;
}

void Server::Run() {
    int socket_fd;  // Socket file descriptor
    
    if (!Init())
        exit(-1);
    if (!Bind())
        exit(-2);
    if (!Listen())
        exit(-3);

    int max_clients = get_nprocs_conf() - 1;
    cout << "SERVER STARTED\n    MAX CLIENTS: " << max_clients << endl;

    while (true) {
        socket_fd = ::accept(socket_fd_, nullptr, nullptr);
        if (socket_fd < 0) {
            cerr << "Socket connection: " << ::strerror(errno) << endl;
            continue;
        }
        cout << "  CLIENT CONNECTED" << endl;

        string message;
        while (true) {
            // Read message from server.
            ::ssize_t bytes_read = Read(&message, socket_fd);
            if (bytes_read < 0) {
                cerr << "Server shutting down..." << endl;
                exit(0);
            }
            else if (bytes_read == 0) {
                cout << "Client disconnected" << endl;
                close(socket_fd);
                break;
            }

            // Split string into vector of arguments: path, line1, line2...
            char us = message[0];
            char eot = message[1];
            message = message.substr(2);
            vector<string> lines = Explode(message, us, eot);

            string path = lines[0];
            lines.erase(lines.begin());  // Remove element 0 to get lines only

            // Print file path and requested lines to console
            cout << "    PATH: " << path << endl;
            cout << "    LINES: ";
            for (size_t i=0; i < lines.size()-1; ++i)
                cout << lines[i] << ", ";
            cout << lines[lines.size()-1] << endl;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << " Usage : " << argv[0] << " <socket name>" << endl;
        exit(-4);
    }
    
    Server server(argv[1]);
    server.Run();

    return 0;
}