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

#define BUFFER_SIZE         16384           // 16KB = 4 pages
#define SHM_PATH            "MP_SHM"

#define SEM_SERVER          "/MP_SEM_SERVER"
#define SEM_CLIENT          "/MP_SEM_CLIENT"

struct shm_buffer {
    char buffer[BUFFER_SIZE];
};

# endif  // PROJ3_SHARED_MEM_H_