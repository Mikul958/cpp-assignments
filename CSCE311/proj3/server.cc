// Copyright 2024 mpikula

// TODO modify for project 3

#include <proj3/server.h>

void DestroySemaphores(int signum) {
    ::sem_unlink(SEM_SERVER);
    ::sem_unlink(SEM_CLIENT);
}

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
    // Create signal handler to destroy named semaphores upon termination
    ::signal(SIGTERM, DestroySemaphores);                                                       // TODO figure out how to include DS in class
    ::signal(SIGINT, DestroySemaphores);

    // Pre-delete named semaphores in case they still exist from past run.
    ::sem_unlink(SEM_SERVER);
    ::sem_unlink(SEM_CLIENT);

    // Open named semaphores for communication, both with count 0.
    sem_t * sem_server = ::sem_open(SEM_SERVER, O_CREAT, 0660, 0);
    sem_t * sem_client = ::sem_open(SEM_CLIENT, O_CREAT, 0660, 0);
    if (sem_server == SEM_FAILED || sem_client == SEM_FAILED) {
        cout << "Server::Run: ::sem_open " << strerror(errno) << endl;
        ::sem_unlink(SEM_SERVER);
        ::sem_unlink(SEM_CLIENT);
        exit(-1);
    }
    cout << "SERVER STARTED" << endl;
    
    while (true) {        
        // Wait on a client to unblock server and open/map shared memory.
        ::sem_wait(sem_server);
        int shm_fd;
        struct shm_buffer * shm_ptr;
        shm_fd = ::shm_open(SHM_PATH, O_RDWR, 0);
        shm_ptr = reinterpret_cast<struct shm_buffer*>(mmap(NULL, sizeof(*shm_ptr), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));

        // STEP 2. Read client message through shared memory.                                         TODO this section is really sketch
        cout << "CLIENT REQUEST RECEIVED" << endl;
        char request[BUFFER_ROW_SIZE];
        snprintf(request, BUFFER_ROW_SIZE, "%s", shm_ptr->message);                                    // TODO use getline instead?
        int num_lines = shm_ptr->lines;
        cout << "PATH: ";
        for (char c : request) {
            if (c == '\n')
                break;
            else
                cout << c;
        }
        cout << endl << "NUMBER OF LINES: " << num_lines << endl;

        // Confirm request received in output stream and get file path.
        //vector<string> request = ParseMessage(message);
        //string path = "hello";

        // STEP 3. Open the shared memory created by client.                                                 TODO figure out opening shm
        // pretend i did shm stuff
        cout << "\tMEMORY OPEN" << endl;

        // STEP 4. Open and read file at desired path.
        //cout << "\tOPENING: " << path << endl;
        //vector<string> retrieved;
        //bool file_read = ReadFile(path, &retrieved);
        //cout << "\tFILE CLOSED" << endl;

        // Check for file IO error and write file lines to shared memory.
        // NOTE: '0' at response[0] indicates error to client, else normal.
        //string response;
        //if (file_read) {
        //    response = string("1") + END_OF_TRANSMISSION;
            // Write retrieved file lines to shared memory.                                                 TODO figure out writing to shm
        //} else {
        //    response = string("0") + END_OF_TRANSMISSION;
        //}
        

        // Release server's hold on shared memory.                                                           TODO figure out closing shm
        // pretend i did shm stuff
        clog << "\tMEMORY CLOSED" << endl;

        // Block until next client opens connection
        
        break;                                                                                              // TODO temporary
    }
}

int main(int argc, char* argv[]) {
    if (argc != 1) {
        cerr << "\n    Usage : " << argv[0] << "\n" << endl;
        exit(-4);
    }

    Server server;
    server.Run();

    return 0;
}
