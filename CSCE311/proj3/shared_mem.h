// Copyright 2023 LittleCube
// Modified by Michael Pikula

#ifndef PROJ3_SHARED_MEM_H_
#define PROJ3_SHARED_MEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

#define END_OF_TRANSMISSION '\004'

#define BUFFER_ROWS         4                // 4 threads, 1 row each
#define BUFFER_ROW_SIZE     524288           // Max expected file size / 4
#define SHM_PATH            "MP_SHM"

#define SEM_SERVER          "/MP_SEM_SERVER"
#define SEM_CLIENT          "/MP_SEM_CLIENT"

// message is main message, contains client request and main server response
// buffer will contain file lines written back by server
struct shm_buffer {
    int lines;
    char message[BUFFER_ROW_SIZE];
    char buffer[BUFFER_ROWS][BUFFER_ROW_SIZE];
};

# endif  // PROJ3_SHARED_MEM_H_