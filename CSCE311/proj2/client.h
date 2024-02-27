// Copyright 2024 mpikula

#ifndef PROJ2_CLIENT_H_
#define PROJ2_CLIENT_H_

#include <proj2/domain_socket.h>
#include <proj2/calculate.h>

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

        std::vector<std::string> Explode(std::string, char = '\037', char = '\004');
        void Run(std::string message);
};

#endif  // PROJ2_CLIENT_H_
