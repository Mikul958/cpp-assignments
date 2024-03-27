// Copyright 2024 mpikula

// TODO modify for project 3

#ifndef PROJ3_SERVER_H_
#define PROJ3_SERVER_H_

#include <proj3/shared_mem.h>

#include <sys/un.h>
#include <unistd.h>
#include <signal.h>

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

void DestroySemaphores(int signum);

/**
* Opens file at the specified path and returns all lines to output vector.
* @param path The path of the file to read.
* @param output Output parameter; contains all lines read from file.
* @return true if file exists and was successfully read.
*/
bool ReadFile(string path, vector<string>* output);

void Run();


#endif  // PROJ3_SERVER_H_
