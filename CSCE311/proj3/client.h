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

    //void *EvaluateSHM(void * t_id);

    void Run(string filepath, int num_lines);
};

#endif  // PROJ3_CLIENT_H_
