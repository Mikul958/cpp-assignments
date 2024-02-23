#ifndef PROJ2_CLIENT_H_
#define PROJ2_CLIENT_H_

#include <proj2/domain_socket.h>

#include <cstddef>
#include <cstdlib>

#include <string>
#include <iostream>


const char kSocketPath[] = "socket_example";


class Client : public DomainSocket {
    public:
        using DomainSocket::DomainSocket;

        void Run();
};

#endif  // PROJ2_CLIENT_H_
