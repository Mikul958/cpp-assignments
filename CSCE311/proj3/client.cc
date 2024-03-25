// Copyright 2024 mpikula

#include <proj3/client.h>

double Client::EvaluateLine(string line) {
    vector<string> equation;

    string current;
    for (char c : line) {
        if (c == ' ') {
            equation.push_back(current);
            current.clear();
        } else {
            current += c;
        }
    }
    if (current.length() > 0)
        equation.push_back(current);

    // Remove "Line" and "<num>:" from equation vector
    equation.erase(equation.begin(), equation.begin()+2);

    return Calculate(equation);  // Using calculate.h from Project 1
}

void Client::Run(string path, int num_lines) {
    // Initalize client and attempt to connect to DomainSocket
    cout << "Client initializing..." << endl;
    if (!Init())
        exit(1);
    cout << "Client connecting..." << endl;
    if (!Connect())
        exit(2);
    cout << "SERVER CONNECTION ACCEPTED" << endl;

    // STEP 1. Create shared memory between this client and the server                                  TODO figure out shm


    // STEP 2. Build message string from request and write to server                                    TODO decide between shm and re-using domain sockets
    string message = path + DomainSocket::kEoT;
    ::ssize_t bytes_written = Write(message);
    if (bytes_written < 0) {
        cerr << "DomainSocketClient terminating..." << endl;
        exit(3);
    } else if (bytes_written == 0) {
        cout << "Server disconnected" << endl;
        exit(4);
    }

    // Read in response from server and exit if error.
    string response;
    Read(&response);
    if (response[0] == '0') {
        cout << "INVALID FILE" << endl;
        // close shared memory                                                                  TODO close shm
        return;
    }


    // STEP 2.5 BEFORE THE REST: read the entirety of shared memory to test server write
    



    // STEP 3. Create 4 threads and determine each section of shared memory.                    TODO figure out pthreads


    // STEP 4. Use threads to evaluate evenly-sized sections of shared memory.


    // STEP 5. Close shared memory (step 6 is in main).                                                                      TODO figure out shm
}

int main(int argc, char* argv[]) {
    // Validate usage
    if (argc != 3) {
        cerr << "\n    Usage : " << argv[0]
             << "  <filepath> <number of lines in file>\n" << endl;
        exit(5);
    }

    // Obtain args from command line and check validity.
    string path = argv[1];
    int num_lines;
    try {
        num_lines = std::stoi(argv[2]);
        if (num_lines < 1)
            throw std::invalid_argument("negative line count");
    }
    catch (const std::invalid_argument &e) {
        cout << "  client: invalid number of lines entered" << endl;
        exit(6);
    }

    // Connect to server with given socket name and request
    Client client("PROJ3_SERVER");
    client.Run(path, num_lines);

    return 0;
}
