// Copyright 2024 Michael Pikula

// TODO modify for project 3

#ifndef PROJ3_SERVER_H_
#define PROJ3_SERVER_H_

#include <proj3/shared_mem.h>

#include <sys/un.h>
#include <unistd.h>
#include <signal.h>  // Signal handler

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
using std::cin;
using std::cerr;
using std::clog;
using std::endl;

// Runs the server.
void Run();

/**
 * Unlinks all named semaphores created by the server
 * @param signum Unused, required to be called by signal handler.
 */
void DestroySemaphores(int signum);

/**
 * Opens file at the specified path and writes lines to shared memory struct.
 * @param path The path of the file to read.
 * @param num_lines The number of lines in the specified file; must be correct.
 * @param output Shared memory location for function to write to.
 */
void ReadFile(string path, int num_lines, struct shm_info * output);

#endif  // PROJ3_SERVER_H_
