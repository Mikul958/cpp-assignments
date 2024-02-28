// Copyright 2024 mpikula

#include <proj2/server.h>

// Open file and read lines at desired line numbers.
// Upon error: returns false, clears output and writes error to output[0].
bool Server::ReadFile(string path, vector<int> lines, vector<string>* output) {
    // Check for invalid file
    std::ifstream file(path.c_str());
    if (!file.is_open()) {
        output->clear();
        output->push_back("INVALID FILE");
        return false;
    }

    // Read lines and count line number, add desired lines to vector
    string line;
    int line_num = 0;
    while (std::getline(file, line)) {
        ++line_num;
        if (std::find(lines.begin(), lines.end(), line_num) != lines.end()) {
            if (line.back() == '\r')
                line.pop_back();     // Clean possible trailing \r from newline
            output->push_back("Line " + std::to_string(line_num) + ": " + line);
        }
    }
    file.close();

    // Check for line requests < 1 or > final line count
    for (int i : lines) {
        if (i < 1 || i > line_num) {
            output->clear();
            output->push_back("INVALID LINE NO " + std::to_string(i));
            return false;
        }
    }

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

    // Get max number of processes on this machine
    int max_clients = get_nprocs_conf() - 1;
    cout << "SERVER STARTED\n    MAX CLIENTS: " << max_clients << endl;

    while (true) {
        // Accept client connection
        if (!Accept(&socket_fd) || socket_fd < 0) {
            cerr << "Socket connection: " << ::strerror(errno) << endl;
            continue;
        }
        cout << "  CLIENT CONNECTED" << endl;

        string message;
        while (true) {
            // Read message from server
            ::ssize_t bytes_read = Read(&message, socket_fd);
            if (bytes_read < 0) {
                cerr << "Server shutting down..." << endl;
                exit(0);
            }
            else if (bytes_read == 0) {
                cout << "  CLIENT DISCONNECTED" << endl;
                close(socket_fd);
                break;
            }



            // Temporary, will not be necessary after refactor                                 TODO refactor message build.
            char eot = DomainSocket::kEoT;



            // Split string into vector of arguments: path, line1, line2...
            vector<string> request = ParseMessage(message);
            string path = request[0];
            request.erase(request.begin());  // Remove item 0 to get lines only
            vector<int> lines;

            // Convert lines to int, return invalid line error if exception
            try {
                for (string s : request)
                    lines.push_back(stoi(s));
            }
            catch (std::invalid_argument& e) {
                string error = "INVALID LINE: NON-NUMERICAL";
                error = error + eot;
                ::ssize_t bytes_written = Write(error, socket_fd, eot);                                             // TODO incorporate BuildMessage
                cout << "      BYTES SENT: " << bytes_written << endl;
                Close(socket_fd);
                break;
            }

            // Print file path and requested lines to console
            cout << "    PATH: \"" << path << "\""<< endl;
            cout << "    LINES: ";
            for (size_t i=0; i < lines.size()-1; ++i)
                cout << lines[i] << ", ";
            cout << lines[lines.size()-1] << endl;


            // TODO figure all this refactoring junk out


            // Obtain desired lines as string vector
            // ReadFile error is returned at retrieved[0] if encountered
            vector<string> retrieved;
            if (!ReadFile(path, lines, &retrieved)) {
                string error = retrieved[0] + kEoT;
                ::ssize_t bytes_written = Write(error, socket_fd, eot);                                 // TODO incorporate BuildMessage
                cout << "      BYTES SENT: " << bytes_written << endl;
                Close(socket_fd);
                break;
            }

            // Build response string and write back to client
            string response = BuildMessage(retrieved);
            ::ssize_t bytes_written = Write(response, socket_fd, eot);
            cout << "      BYTES SENT: " << bytes_written << endl;
            Close(socket_fd);
            break;
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