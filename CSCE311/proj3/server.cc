// Copyright 2024 Michael Pikula

#include <proj3/server.h>

void Run() {
    // Create signal handler to destroy named semaphores upon termination
    ::signal(SIGTERM, DestroySemaphores);
    ::signal(SIGINT, DestroySemaphores);

    // Pre-delete named semaphores in case they still exist from past run.
    DestroySemaphores(0);

    // Open named semaphores for communication, both with count 0.
    sem_t * sem_server = ::sem_open(SEM_SERVER, O_CREAT, 0660, 0);
    sem_t * sem_client = ::sem_open(SEM_CLIENT, O_CREAT, 0660, 0);
    if (sem_server == SEM_FAILED || sem_client == SEM_FAILED) {
        cout << "Server::Run: ::sem_open " << strerror(errno) << endl;
        DestroySemaphores(0);
        exit(-1);
    }
    cout << "SERVER STARTED" << endl;

    while (true) {
        // STEP 2. Wait on a client to unblock server.
        ::sem_wait(sem_server);
        cout << "CLIENT REQUEST RECEIVED" << endl;

        // STEP 3. Open shared memory.
        int shm_fd;
        struct shm_info * shm_ptr;
        shm_fd = ::shm_open(SHM_PATH, O_RDWR, 0);
        shm_ptr = reinterpret_cast<struct shm_info *>
                  (mmap(NULL, sizeof(*shm_ptr),
                        PROT_READ | PROT_WRITE,
                        MAP_SHARED, shm_fd, 0));
        cout << "\tMEMORY OPEN" << endl;

        // Read client message from shared memory.
        string path = shm_ptr->message;
        int num_lines = shm_ptr->num;

        // STEP 4. Read file and write to shared memory.
        cout << "\tOPENING: " << path << endl;
        ReadFile(path, num_lines, shm_ptr);     // Handles response to client
        cout << "\tFILE CLOSED" << endl;

        // STEP 5. Unblock client and close shared memory.
        ::sem_post(sem_client);
        ::munmap(shm_ptr, sizeof(struct shm_info));
        ::close(shm_fd);
        clog << "\tMEMORY CLOSED" << endl;
    }
}

void DestroySemaphores(int signum) {
    ::sem_unlink(SEM_SERVER);
    ::sem_unlink(SEM_CLIENT);
}

void ReadFile(string path, int num_lines, struct shm_info * output) {
    // Check for invalid file path.
    std::ifstream file(path.c_str());
    if (!file.is_open()) {
        output->num = BAD_READ;
        string error = "INVALID FILE";
        ::strncpy(output->message, error.c_str(), MESSAGE_SIZE);
        return;
    }

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
        // Check line number against segment bound, increment segment if >=
        if (line_number >= ends[segment]) {
            segment++;
            offset = 0;
            if (segment > 3) {
                line_number++;    // Ensure final error check fails
                break;
            }
        }

        // Clean potential trailing \r and add newline delimiter.
        if (line.back() == '\r')
            line.pop_back();
        line += '\n';

        // Append line to main shared memory buffer and update information.
        // NOTE: offset + BUFFER_ROW_SIZE - offset = BUFFER_ROW_SIZE
        ::strncpy(output->buffer[segment] + offset, line.c_str(),
                                                    BUFFER_ROW_SIZE - offset);
        offset += line.size();
        line_number++;
    }
    file.close();

    // Check discrepancy between entered line number and actual amount
    if (line_number != num_lines) {
        output->num = BAD_READ;
        string error = "INCORRECT NUMBER OF LINES ENTERED";
        ::strncpy(output->message, error.c_str(), MESSAGE_SIZE);
    }
}

int main(int argc, char* argv[]) {
    // Validate arguments.
    if (argc != 1) {
        cerr << "\n    Usage : " << argv[0] << "\n" << endl;
        exit(-2);
    }

    // Run server.
    Run();
    return 0;
}
