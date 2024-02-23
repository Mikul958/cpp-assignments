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
    if (argc < 4) {
        cerr << "Must provide socket name, file path, and file lines\n"
             << "Ex. ./client socket_name file_name line_0 ... line_n" << endl;
        exit(5);
    }
    
    char* socket_name = argv[1];
    
    string file_path = argv[2];
    std::vector<string> line_numbers;
    for (int i=3; i < argc-3; ++i)
        line_numbers.push_back(argv[i]);

    Client client(socket_name);
    client.Run();

    exit(0);
}