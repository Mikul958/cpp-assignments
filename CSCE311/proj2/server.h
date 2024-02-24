// Copyright 2024 mpikula

#ifndef PROJ2_SERVER_H_
#define PROJ2_SERVER_H_

#include <proj2/domain_socket.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/sysinfo.h>

#include <cassert>
#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <cstring>

#include <string>
#include <iostream>

class Server : public DomainSocket {
    public:
        using ::DomainSocket::DomainSocket;

        void Run();
};

#endif  // PROJ2_SERVER_H_
