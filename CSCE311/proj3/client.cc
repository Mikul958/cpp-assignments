// Copyright 2024 Michael Pikula

#include <proj3/client.h>

void Run(string path, int num_lines) {
    // STEP 1. Create shared memory and store pointer to its location.
    struct shm_info * shm_ptr = CreateSHM();
    cout << "SHARED MEMORY ALLOCATED: " << sizeof(struct shm_info)
         << " BYTES" << endl;

    // Open named semaphores created by server
    sem_t * sem_server = ::sem_open(SEM_SERVER, 0);
    sem_t * sem_client = ::sem_open(SEM_CLIENT, 0);

    // STEP 2. Write file path to shared memory message and num_lines to num
    ::strncpy(shm_ptr->message, path.c_str(), MESSAGE_SIZE);
    shm_ptr->num = num_lines;

    // Unblock server, block client until server is done writing back.
    ::sem_post(sem_server);
    ::sem_wait(sem_client);

    // Read in main server response and check for errors.
    int status = shm_ptr->num;
    if (status == BAD_READ) {
        string error = shm_ptr->message;
        cout << error << endl;
        DestroySHM(shm_ptr);
        return;
    }

    // STEP 3. Create 4 threads, each evaluating 1/4 of shared memory.
    struct thread_args t_args_array[4];
    for (int i=0; i < 4; i++) {
        t_args_array[i].data = shm_ptr;    // Pass in pointer to shm
        t_args_array[i].segment = i;       // Pass in segment to evaluate
    }

    pthread_t threads[4];
    for (pthread_t t_id=0; t_id < 4; t_id++)
        ::pthread_create(&threads[t_id], NULL, EvaluateSHM,
                         reinterpret_cast<void *>(&t_args_array[t_id]));
    cout << "THREADS CREATED" << endl;

    // Wait on threads to finish executing.
    for (pthread_t t_id=0; t_id < 4; t_id++)
        ::pthread_join(threads[t_id], NULL);

    // STEP 4. Print results of each thread and sum to console.
    std::locale comma_structure("");              // Use system default commas.
    cout.imbue(comma_structure);
    cout << std::fixed << std::setprecision(2);   // Long-form numbers.

    // Print thread sums and add to running total.
    double total = 0;
    for (int i=0; i < 4; i++) {
        struct thread_args current = t_args_array[i];
        total += current.sum;
        cout << "THREAD " << i << ":  " << current.operations
             << " LINES, " << current.sum << endl;
    }
    cout << "SUM:  " << total << endl;

    // STEP 5. Unmap and destroy shared memory.
    DestroySHM(shm_ptr);
}

struct shm_info * CreateSHM() {
    int shm_fd;
    struct shm_info * shm_ptr;    // Pointer to mapped area of shared memory.
    ::shm_unlink(SHM_PATH);       // Pre-delete in case of previous error.

    // Open new shared memory location.
    shm_fd = ::shm_open(SHM_PATH, O_CREAT | O_EXCL | O_RDWR,
                                  S_IRUSR | S_IWUSR);
    if (shm_fd == -1) {
        cerr << "client.cc::Run: failed to create shared memory" << endl;
        exit(2);
    }

    // Set size of shared memory location using shm_info struct.
    if (::ftruncate(shm_fd, sizeof(struct shm_info)) == -1) {
        cerr << "client.cc::Run: failed to set size of shared memory" << endl;
        ::shm_unlink(SHM_PATH);
        exit(3);
    }

    // Map shared memory and return location at shm_ptr.
    shm_ptr = reinterpret_cast<struct shm_info*>(mmap(NULL, sizeof(*shm_ptr),
                                                   PROT_READ | PROT_WRITE,
                                                   MAP_SHARED, shm_fd, 0));
    if (shm_ptr == MAP_FAILED) {
        cerr << "client.cc::Run: failed to map shared memory" << endl;
        ::shm_unlink(SHM_PATH);
        exit(4);
    }

    // Return pointer to shared memory location.
    return shm_ptr;
}

void DestroySHM(struct shm_info * shm_ptr) {
    ::munmap(shm_ptr, sizeof(struct shm_info));
    ::shm_unlink(SHM_PATH);
}

void * EvaluateSHM(void * input) {
    // Cast void pointer back to struct to get parameters.
    struct thread_args * args = reinterpret_cast<struct thread_args *>(input);
    struct shm_info * shm_ptr = args->data;
    int row = args->segment;

    // Evaluate lines from shared memory buffer.
    int start = 0, current;
    char * buffer_row = shm_ptr->buffer[row];
    for (current=0; buffer_row[current] != '\0'; current++) {
        if (buffer_row[current] == '\n') {
            // Hit newline, store line in string and update start for next.
            string file_line;
            for (int i=start; i < current; i++)
                file_line += buffer_row[i];
            start = current + 1;

            // Update operation count and sum.
            args->operations++;
            args->sum += EvaluateLine(file_line);
        }
    }

    // Return with exit code 0.
    ::pthread_exit(0);
}

double EvaluateLine(string line) {
    vector<string> equation;

    // Split line at spaces and add pieces to equations vector.
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

int main(int argc, char* argv[]) {
    // Validate arguments.
    if (argc != 3) {
        cerr << "\n    Usage : " << argv[0]
             << "  <filepath> <number of lines in file>\n" << endl;
        exit(1);
    }

    string path = argv[1];
    int num_lines;
    try {
        num_lines = std::stoi(argv[2]);
        if (num_lines < 1)
            throw std::invalid_argument("negative line count");
    }
    catch (const std::invalid_argument &e) {
        cerr << "INVALID NUMBER OF LINES ENTERED" << endl;
        exit(1);
    }

    // Run client.
    Run(path, num_lines);

    // STEP 6. Return 0 to indicate nominative exit status.
    return 0;
}
