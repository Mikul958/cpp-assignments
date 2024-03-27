// Copyright 2024 mpikula

// TODO modify for project 3

#include <proj3/server.h>

void DestroySemaphores(int signum) {
    ::sem_unlink(SEM_SERVER);
    ::sem_unlink(SEM_CLIENT);
}

int ReadFile(string path, int num_lines, struct shm_buffer * output) {
    // Check for invalid file path
    std::ifstream file(path.c_str());
    if (!file.is_open())
        return BAD_READ;

    // Determine how many lines go in each shared memory segment.
    int ends[4];
    int range = num_lines / 4;
    int remainder = num_lines % 4;
    for (int i=0; i < 4; i++)
        ends[i] = range * (i+1);
    for (int i=0; i < remainder; i++)
        ends[3-i] += remainder - i;

    // Read all file lines and write to respective segment of shared memory
    int line_number = 0, segment = 0, offset = 0;
    string line;
    while (std::getline(file, line)) {
        // Check segment, move to next segment if necessary
        if (line_number >= ends[segment]) {
            segment++;
            if (segment > 3)
                return BAD_READ;
            offset = 0;
        }
        
        // Prepare line for output.
        if (line.back() == '\r')
            line.pop_back();
        line += '\n';

        // Copy line to output buffer
        ::strncpy(output->buffer[segment] + offset, line.c_str(), BUFFER_ROW_SIZE);
        offset += line.size();
        line_number++;
    }
    file.close();

    // Check discrepancy between entered line number and actual amount
    if (line_number != num_lines)
        return BAD_READ;

    return 0;
}

void Run() {
    // Create signal handler to destroy named semaphores upon termination
    ::signal(SIGTERM, DestroySemaphores);
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

        // STEP 2 AND 3. Read client message through shared memory.
        cout << "CLIENT REQUEST RECEIVED\n\tMEMORY OPEN" << endl;
        char request[MESSAGE_SIZE];
        //cin.getline(shm_ptr->message, request);
        snprintf(request, MESSAGE_SIZE, "%s", shm_ptr->message);                                    // TODO use getline instead? (line above, broken atm)

        string path = request;
        path.pop_back();                // Pop newline char
        int num_lines = shm_ptr->num;

        // STEP 4. Read file and write to shared memory.
        cout << "\tOPENING: " << path << endl;
        int status = ReadFile(path, num_lines, shm_ptr);
        cout << "\tFILE CLOSED" << endl;



        if (status == BAD_READ)                                                                     // TODO have ReadFile send error through shm
            cout << "BAD_READ" << endl;



        clog << "\tMEMORY CLOSED" << endl;                                                                 // TODO actually close the memory

        
        // Unblock client                                                                                   TODO possible temporary
        ::sem_post(sem_client);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 1) {
        cerr << "\n    Usage : " << argv[0] << "\n" << endl;
        exit(-4);
    }

    Run();

    return 0;
}
