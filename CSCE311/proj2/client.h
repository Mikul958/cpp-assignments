#ifndef PROJ2_CLIENT_H_
#define PROJ2_CLIENT_H_

#include <proj2/domain_socket.h>

#include <cstddef>
#include <cstdlib>

#include <string>
#include <iostream>

class Client : public DomainSocket {
    public:
        using DomainSocket::DomainSocket;

        void Run(std::string message);
};

#endif  // PROJ2_CLIENT_H_
