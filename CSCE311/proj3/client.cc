// Copyright 2024 Michael Pikula

#include <proj3/client.h>

double EvaluateLine(string line) {
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

    return Calculate(equation);  // Using calculate.h from Project 1
}

void *EvaluateSHM(void * input) {
    // Cast void pointer back to struct to get parameters.
    //struct thread_args * args = (struct thread_args *) input;
    //vector<string> data = *(args->data);
    
    // Evaluate designated of shared memory and add to section total.
    //for (int i=args->start; i < args->end; i++)
    //    args->sum += EvaluateLine(data[i]);

    pthread_exit(0);
}

void Run(string path, int num_lines) {

    // STEP 1. Create shared memory between this client and the server                                              TODO move to createSharedMem function?
    int shm_fd;
    struct shm_info * shm_ptr;  // Pointer to mapped area of shared memory
    ::shm_unlink(SHM_PATH);       // Pre-delete in case of previous error.

    // Open shared memory
    shm_fd = ::shm_open(SHM_PATH, O_CREAT | O_EXCL | O_RDWR,
                                  S_IRUSR | S_IWUSR);
    if (shm_fd == -1) {
        cerr << "Client::Run: failed to create shared memory, may already exist" << endl;
        return;
    }

    // Set size of shared memory buffer.
    if(::ftruncate(shm_fd, sizeof(struct shm_info)) == -1 ) {
        cerr << "Client::Run: failed to set size of shared memory object" << endl;
        ::shm_unlink(SHM_PATH);
        return;
    }
    cout << "SHARED MEMORY ALLOCATED: " << sizeof(struct shm_info) << " BYTES" << endl;

    // Map shared memory and return location at shm_ptr.
    shm_ptr = reinterpret_cast<struct shm_info*>(mmap(NULL, sizeof(*shm_ptr),
                                                   PROT_READ | PROT_WRITE,
                                                   MAP_SHARED, shm_fd, 0));
    if (shm_ptr == MAP_FAILED) {
        cerr << "Client::Run: failed to map shared memory" << endl;
        ::shm_unlink(SHM_PATH);
        return;
    }

    // Open named semaphores created by server
    sem_t * sem_server = ::sem_open(SEM_SERVER, 0);
    sem_t * sem_client = ::sem_open(SEM_CLIENT, 0);

    // STEP 2. Write message to shared memory buffer and num_lines to num
    string request = path + '\n';
    ::strncpy(shm_ptr->message, request.c_str(), MESSAGE_SIZE);
    shm_ptr->num = num_lines;

    // Unblock server, block client until server is done writing back.
    ::sem_post(sem_server);
    ::sem_wait(sem_client);

    // Read in main server response and check for errors.                                                   TODO integrate with threads
    int status = shm_ptr->num;
    if (status == BAD_READ) {
        char error[MESSAGE_SIZE];
        ::snprintf(error, MESSAGE_SIZE, "%s", shm_ptr->message);
        cout << "Client::Run: " << error << endl;
        ::munmap(shm_ptr, sizeof(struct shm_info));
        ::shm_unlink(SHM_PATH);
        return;
    }

    // STEP 3. Create 4 threads, each evaluating 1/4 of shared memory.
    
    




    // Create 4 threads executing EvaluateResult(t_args_array) and wait.
    

    // STEP 5. Unmap and close shared memory.                                                                              TODO move to cleanup function
    ::munmap(shm_ptr, sizeof(struct shm_info));
    ::shm_unlink(SHM_PATH);
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
    Run(path, num_lines);

    return 0;
}
