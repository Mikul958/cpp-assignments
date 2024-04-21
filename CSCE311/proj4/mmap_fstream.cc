// Copyright 2024 Michael Pikula

#include <proj4/mmap_fstream.h>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

namespace mem_map {

fstream::fstream(const string &filepath, ios_base::openmode mode) {
    filename_ = filepath;
    open_mode_ = mode;
    
    // All default values, properly set in open() if open successful
    file_descriptor_ = -1;
    cursor_ = -1;
    file_size_ = -1;
    is_open_ = false;
    end_of_file_ = false;
    buffer_ptr_ = nullptr;

    // Only open file if name was given
    if (filename_ != "")
        open(filename_);
}

fstream::~fstream() {
    close();
}

void fstream::open(const string &filepath, ios_base::openmode mode) {
    // Check if a file is already open, do nothing if yes
    if (is_open_)
        return;

    // Initalize permissions (S_..., PROT_... and MAP_... respectively)
    int open_perms = 0000;
    int prot_perms = 0;
    int map_perms = 0;

    // Check openmode's in/out and update permissions
    if (mode & ios_base::in) {
        open_perms = open_perms | S_IRUSR | S_IRGRP;
        prot_perms = prot_perms | PROT_READ;
        map_perms = MAP_PRIVATE;
    }
    if (mode & ios_base::out) {
        open_perms = open_perms | S_IWUSR | S_IWGRP;
        prot_perms = prot_perms | PROT_WRITE;
        map_perms = MAP_SHARED;
    }

    // Open file and store file descriptor
    file_descriptor_ = ::open(filepath.c_str(), O_CREAT | O_RDWR, open_perms);
    if (file_descriptor_ == -1) {
        cerr << "fstream::open(): " << strerror(errno) << endl;
        exit (-1);
    }

    // Get file size using stat.h and file descriptor
    struct stat file_stats;
    fstat(file_descriptor_, &file_stats);
    file_size_ = file_stats.st_size;

    // If size is 0, file is new or was empty, default to 1 page of memory.
    if (file_size_ == 0) {
        const int kPageSize = 4096;                      // 4KB = 1 page
        if (::ftruncate(file_descriptor_, kPageSize)) {
            cout << "fstream::open(): " << strerror(errno) << endl;
            ::close(file_descriptor_);
            exit(-2);
        }
        file_size_ = kPageSize;
    }

    // Map file to created memory location.                                                     TODO
    

    is_open_ = true;
}

void fstream::close() {
    if (!is_open_)
        return;
    
    // stuff                                                                                    TODO

    is_open_ = false;
}

// Set to true on file open, set to false on file close
bool fstream::is_open() const {
    return is_open_;
}

// Set to true if cursor is >= file size
bool fstream::eof() const {
    return end_of_file_;
}

std::size_t fstream::size() const {
    return file_size_;
}

char fstream::get() {
    // Get next character (update cursor)
}

fstream& fstream::getline(string* line) {
    // Call get until newline character or end of file
}

fstream& fstream::put(char c) {
    // Add character to end of file, MAY CHANGE FILE SIZE, MAKE SURE TO UPDATE
}

}  // namespace mem_map
