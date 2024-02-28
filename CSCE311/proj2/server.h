// Copyright 2024 mpikula

#ifndef PROJ2_SERVER_H_
#define PROJ2_SERVER_H_

#include <proj2/domain_socket.h>

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
        
        // Reads file and outputs resulting lines / error to a vector
        bool ReadFile(string, vector<int>, vector<string>*);

        void Run();
};

#endif  // PROJ2_SERVER_H_
