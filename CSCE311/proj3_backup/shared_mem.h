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

#define MESSAGE_SIZE        4096            // 4KB, anything over is ridiculous
#define THREAD_COUNT        4               
#define BUFFER_ROW_SIZE     524288          // 2^19 bytes, more than enough
#define SHM_PATH            "MP_SHM"

#define BAD_READ            -1

#define SEM_SERVER          "/MP_SEM_SERVER"
#define SEM_CLIENT          "/MP_SEM_CLIENT"


struct shm_info {
    int num;                                    // Lines/error status
    char message[MESSAGE_SIZE];                 // Request path/potential error
    char buffer[THREAD_COUNT][BUFFER_ROW_SIZE]; // Main buffer
};

# endif  // PROJ3_SHARED_MEM_H_