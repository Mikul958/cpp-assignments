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
using std::endl;

class Server : public DomainSocket {
 public:
    using ::DomainSocket::DomainSocket;

    // Reads file and outputs resulting lines to output vector.
    // Returns false and outputs error to output[0] if it fails.
    bool ReadFile(string path, vector<int> lines, vector<string>* output);

    // Builds error string to send to client from a message.
    // Error message built as "0 + US + <error message> + EoT"
    string ToError(string message);

    void Run();
};

#endif  // PROJ3_SERVER_H_
