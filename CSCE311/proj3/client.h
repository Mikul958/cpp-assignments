// Copyright 2024 Michael Pikula

// TODO modify for project 3

#ifndef PROJ3_CLIENT_H_
#define PROJ3_CLIENT_H_

#include <proj3/shared_mem.h>    // Contains shared mem path & buffer info
#include <proj3/calculate.h>     // Contains calculate functions

#include <sys/unistd.h>  // UNIX standard header
#include <sys/mman.h>    // Shared memory functions
#include <fcntl.h>       // O_CREAT, ...
#include <string.h>      // strlen, write
#include <pthread.h>     // POSIX threads

#include <cassert>
#include <cerrno>
#include <cstddef>
#include <cstdlib>

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;

using std::cout;
using std::cerr;
using std::endl;


// Trims and splits a line from the server and evaluates its equation
double EvaluateLine(string line);

/**
* Evaluates the specified range of shared memory and returns sum of results
* Intended to be called through EvaluateResultHelper() from pthreads
* @param input struct containing pointer to string<vector>, int start,
*              int end, and double out; sum of section is returned at out.
*/
void *EvaluateSHM(void * input);

// Struct containing args for EvaluateSHM
struct thread_args {
    struct shm_info * data;
    int segment;
    double sum = 0;
}; 

void Run(string filepath, int num_lines);

#endif  // PROJ3_CLIENT_H_
