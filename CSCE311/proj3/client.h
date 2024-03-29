// Copyright 2024 Michael Pikula

#ifndef PROJ3_CLIENT_H_
#define PROJ3_CLIENT_H_

#include <proj3/shared_mem.h>    // Contains shared mem path & buffer info
#include <proj3/calculate.h>     // Contains calculate functions

#include <sys/unistd.h>     // UNIX standard header
#include <pthread.h>        // POSIX threads

#include <string>
#include <iostream>
#include <locale>           // Make client output pretty

using std::string;

using std::cout;
using std::cerr;
using std::endl;

// Struct containing arguments for EvaluateSHM
struct thread_args {
    struct shm_info * data;  // Pointer to mapped shared memory struct
    int segment;             // Row of shared memory buffer to evaluate
    int operations = 0;      // Amount of lines evaluated; output parameter
    double sum = 0;          // Total for thread; output parameter
};

/**
 * Runs the client.
 * @param filepath The path of the file for the server to read from.
 * @param num_lines The number of lines in the specified file; must be correct.
 */
void Run(string filepath, int num_lines);

/**
 * Creates and maps a shared memory location for the client and server.
 * @return Pointer to the start of shared memory struct.
 */
struct shm_info * CreateSHM();

/**
 * Unmaps and unlinks shared memory location.
 * @param shm_ptr Pointer to shared memory location.
 */
void DestroySHM(struct shm_info * shm_ptr);

/**
 * Evaluates the specified segment of shared memory buffer.
 * Intended to be called via pthreads.
 * @param input struct thread_args containing pointer to shared memory, row of
 *              shm buffer, number of operations (output), and sum (output).
 */
void * EvaluateSHM(void * input);

/**
 * Evaluates the equation contained in the specified line.
 * @param line Line to be evaluated.
*/
double EvaluateLine(string line);

#endif  // PROJ3_CLIENT_H_
