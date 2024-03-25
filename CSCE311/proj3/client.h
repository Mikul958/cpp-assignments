// Copyright 2024 mpikula

// TODO modify for project 3

#ifndef PROJ3_CLIENT_H_
#define PROJ3_CLIENT_H_

#include <proj3/domain_socket.h>  // TODO probably obsolete
#include <proj3/calculate.h>

#include <sys/mman.h>  // shared memory
#include <sys/unistd.h>  // UNIX standard header
#include <pthread.h>  // POSIX threads

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

class Client : public DomainSocket {
 public:
    using DomainSocket::DomainSocket;

    // Trims and splits a line from the server and evaluates its equation
    double EvaluateLine(string line);

    /**
    * Evaluates the specified range of shared memory and returns sum of results
    * Intended to be called through pthreads
    * @param input struct containing pointer to string<vector>, int start,
    *              int end, and double out; sum of section is returned at out.
    */
    void *EvaluateSHM(void * t_id);

    // Wrapper for pthread to call EvaluateSHM
    static void *EvaluateSHM_Helper(void * t_id) {
        return ((Client *)t_id)->EvaluateSHM(t_id);
    }

    // Struct containing args for EvaluateSHM
    struct thread_args {
        vector<string> * data;
        int start = 0;
        int end = 0;
        double sum = 0;
    }; 

    void Run(string filepath, int num_lines);
};

#endif  // PROJ3_CLIENT_H_
