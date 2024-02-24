// Copyright 2024 mpikula

#include <proj2/server.h>

using std::cout;
using std::cerr;
using std::endl;

using std::string;

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

        while (true) {
            string message;
            ::ssize_t bytes_read = Read(&message, socket_fd);

            if (bytes_read < 0) {
                cerr << "Server shutting down..." << endl;
                exit(0);
            }
            else if (!bytes_read) {
                cout << "Client disconnected" << endl;
                close(socket_fd);
                break;
            }

            std::cout << "read " << bytes_read << " bytes: ";
            std::cout << message << endl;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Must provide socket name\nEx. ./server socket_name" << endl;
        exit(-4);
    }
    
    Server server(argv[1]);
    server.Run();

    return 0;
}