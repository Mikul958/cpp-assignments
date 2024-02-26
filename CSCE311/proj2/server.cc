// Copyright 2024 mpikula

#include <proj2/server.h>

// Split string into vector of args based on provided separator
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

// Open file and read lines at desired line numbers.
// Returns false if file not found
bool Server::ReadFile(string path, vector<int> lines, vector<string>* out) {
    std::ifstream file(path.c_str());
    if (!file.is_open())
        return false;
    
    cout << "      TEST PRINT: READING FROM: " << path << endl;

    out->push_back(string("test"));
    return true;
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
            vector<string> request = Explode(message, us, eot);

            string path = request[0];
            request.erase(request.begin());  // Remove item 0 to get lines only
            vector<int> lines;
            for (string s : request)
                lines.push_back(stoi(s));

            // Print file path and requested lines to console
            cout << "    PATH: \"" << path << "\""<< endl;
            cout << "    LINES: ";
            for (size_t i=0; i < lines.size()-1; ++i)
                cout << lines[i] << ", ";
            cout << lines[lines.size()-1] << endl;

            // Obtain desired lines as vector of strings from file.
            // TODO I don't know how to close client connection
            vector<string> retrieved;
            if (!ReadFile(path, lines, &retrieved)) {
                cout << "      TEST PRINT: INVALID FILE\n";
                close(socket_fd);
                break;
            }
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