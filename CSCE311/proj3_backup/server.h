// Copyright 2024 mpikula

// TODO modify for project 3

#ifndef PROJ3_SERVER_H_
#define PROJ3_SERVER_H_

#include <proj3/domain_socket.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/sysinfo.h>  // Using get_nprocs_conf

#include <cassert>
#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <cstring>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>  // Using ifstream to read file

using std::string;
using std::vector;

using std::cout;
using std::cerr;
using std::clog;
using std::endl;

class Server : public DomainSocket {
 public:
    using ::DomainSocket::DomainSocket;

    /**
    * Opens file at the specified path and returns all lines to output vector.
    * @param path The path of the file to read.
    * @param output Output parameter; contains all lines read from file.
    * @return true if file exists and was successfully read.
    */
    bool ReadFile(string path, vector<string>* output);

    void Run();
};

#endif  // PROJ3_SERVER_H_
