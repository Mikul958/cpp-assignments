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

    return Calculate(equation);  // Using calculate.h from Project 1
}

void *Client::EvaluateSHM(void * input) {
    // Cast void pointer back to struct to get parameters.
    struct thread_args * args = (struct thread_args *) input;
    vector<string> data = *(args->data);
    
    // Evaluate designated of shared memory and add to section total.
    for (int i=args->start; i < args->end; i++)
        args->sum += EvaluateLine(data[i]);

    pthread_exit(0);
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

    // STEP 1. Create shared memory between this client and the server                                              TODO move to createSharedMem function?
    int shm_fd;
    struct shm_buffer * shm_ptr;  // Pointer to mapped area of shared memory
    ::shm_unlink(SHM_PATH);       // Pre-delete in case of previous error.

    // Open shared memory
    shm_fd = ::shm_open(SHM_PATH, O_CREAT | O_EXCL | O_RDWR,
                                  S_IRUSR | S_IWUSR);
    if (shm_fd == -1) {
        cerr << "Client::Run: failed to create shared memory, may already exist" << endl;
        return;
    }

    // Set size of shared memory buffer (1.2 MB, max expected message size).
    if(::ftruncate(shm_fd, sizeof(struct shm_buffer)) == -1 ) {
        cerr << "Client::Run: failed to set size of shared memory object" << endl;
        ::shm_unlink(SHM_PATH);
        return;
    }
    cout << "SHARED MEMORY ALLOCATED: " << sizeof(struct shm_buffer) << " BYTES" << endl;

    // Map shared memory and return location at shm_ptr.
    shm_ptr = reinterpret_cast<struct shm_buffer*>(mmap(NULL, sizeof(*shm_ptr),
                                                   PROT_READ | PROT_WRITE,
                                                   MAP_SHARED, shm_fd, 0));
    if (shm_ptr == MAP_FAILED) {
        cerr << "Client::Run: failed to map shared memory" << endl;
        ::shm_unlink(SHM_PATH);
        return;
    }

    // STEP 2. Build message string from request and write to server
    string message = path + DomainSocket::kEoT;
    ::ssize_t bytes_written = Write(message);
    if (bytes_written < 0) {
        cerr << "DomainSocketClient terminating..." << endl;
        exit(3);
    } else if (bytes_written == 0) {
        cout << "Server disconnected" << endl;
        exit(4);
    }

    // Read in server response from domain socket and exit if error.
    string response;
    Read(&response);
    if (response[0] == '0') {
        cout << "INVALID FILE" << endl;
        ::munmap(shm_ptr, sizeof(struct shm_buffer));
        ::shm_unlink(SHM_PATH);
        return;
    }

    // Read in lines from shared memory vector and populate equations vector.                                    TODO


    // STEP 3. Create 4 threads, each evaluating 1/4 of equations vector.
    int range = num_lines / 4;
    int remainder = num_lines % 4;
    struct thread_args t_args_array[4];

    // Obtain upper and lower bounds for each thread based on num_lines.
    for (int i=0; i < 4; i++)
        t_args_array[i].end = (i+1) * range;
    for (int i=0; i < remainder; i++)
        t_args_array[3-i].end += remainder-i;
    t_args_array[0].start = 0;
    for (int i=1; i < 4; i++)
        t_args_array[i].start = t_args_array[i-1].end;
    


    // TEST CODE: VECTOR NOT FINAL                                                                                  TODO remove when ready
    vector<string> equations;
    for (int i=0; i<num_lines; i++)
        equations.push_back(std::to_string(i));

    for (int i=0; i<4; i++)
        t_args_array[i].data = &equations;  // Allow each thread to reference equations vector



    // Create 4 threads executing EvaluateSHM(t_args_array) and wait on finish.
    pthread_t threads[4];
    for (pthread_t t_id=0; t_id < 4; t_id++)
        pthread_create(&threads[t_id],
                       NULL,
                       &Client::EvaluateSHM_Helper,
                       (void *) &t_args_array[t_id]);
    cout << "THREADS CREATED" << endl;
    for (pthread_t t_id=0; t_id < 4; t_id++)
        pthread_join(threads[t_id], NULL);
    
    // STEP 4. Report results of threads to console.
    double total = 0;
    for (int i=0; i < 4; i++) {
        struct thread_args current = t_args_array[i];
        cout << "THREAD " << i << ":  " << current.end - current.start
             << " LINES, " << current.sum << endl;
        total += current.sum;
    }
    cout << "SUM:  " << total << endl;

    // STEP 5. Unmap and close shared memory.                                                                              TODO move to cleanup function
    ::munmap(shm_ptr, sizeof(struct shm_buffer));
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
    Client client("MP_SOCKET");
    client.Run(path, num_lines);

    return 0;
}
