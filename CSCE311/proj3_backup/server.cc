// Copyright 2024 mpikula

// TODO modify for project 3

#include <proj3/server.h>

bool Server::ReadFile(string path, vector<string>* output) {
    // Check for invalid file path
    std::ifstream file(path.c_str());
    if (!file.is_open())
        return false;

    // Read all file lines and add to output vector
    string line;
    while (std::getline(file, line)) {
        if (line.back() == '\r')
            line.pop_back();     // Clean possible trailing \r from newline
        output->push_back(line);
    }
    file.close();

    return true;
}

void Server::Run() {
    int socket_fd;  // Socket file descriptor

    // Establish domain socket and begin listening for clients
    if (!Init())
        exit(-1);
    if (!Bind())
        exit(-2);
    if (!Listen())
        exit(-3);
    cout << "SERVER STARTED" << endl;

    while (true) {
        // Accept client connection using domain socket
        if (!Accept(&socket_fd) || socket_fd < 0) {
            cerr << "Socket connection: " << ::strerror(errno) << endl;
            continue;
        }
        cout << "CLIENT CONNECTED" << endl;

        while (true) {
            // STEP 2. Read client message through domain socket.
            string message;
            ::ssize_t bytes_read = Read(&message, socket_fd);
            if (bytes_read < 0) {
                exit(0);
            } else if (bytes_read == 0) {
                close(socket_fd);
                break;
            }

            // Confirm request received in output stream and get file path.
            cout << "CLIENT REQUEST RECEIVED" << endl;
            vector<string> request = ParseMessage(message);
            string path = request[0];

            // STEP 3. Open the shared memory created by client.                                                 TODO figure out opening shm
            // pretend i did shm stuff
            cout << "\tMEMORY OPEN" << endl;

            // STEP 4. Open and read file at desired path.
            cout << "\tOPENING: " << path << endl;
            vector<string> retrieved;
            bool file_read = ReadFile(path, &retrieved);
            cout << "\tFILE CLOSED" << endl;

            // Check for file IO error and write file lines to shared memory.
            // NOTE: '0' at response[0] indicates error to client, else normal.
            string response;
            if (file_read) {
                response = string("1") + DomainSocket::kEoT;
                // Write retrieved file lines to shared memory.                                                 TODO figure out writing to shm
            } else {
                response = string("0") + DomainSocket::kEoT;
            }
            // NOTE: Client blocks waiting for server response, so response
            // must be written after server is done writing to shared memory
            Write(response, socket_fd);

            // Release server's hold on shared memory.                                                           TODO figure out closing shm
            // pretend i did shm stuff
            clog << "\tMEMORY CLOSED" << endl;

            // Close client connection.
            Close(socket_fd);
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 1) {
        cerr << "\n    Usage : " << argv[0] << "\n" << endl;
        exit(-4);
    }

    Server server("MP_SOCKET");
    server.Run();

    return 0;
}
