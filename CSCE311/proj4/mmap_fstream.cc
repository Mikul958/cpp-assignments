// Copyright 2024 Michael Pikula

#include <proj4/mmap_fstream.h>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

namespace mem_map {

fstream::fstream(const string &filepath, ios_base::openmode mode) {
    // All default values, properly set in open() if open successful
    file_descriptor_ = -1;
    cursor_ = -1;
    file_size_ = -1;
    is_open_ = false;
    end_of_file_ = false;
    file_info_ptr_ = nullptr;
    pages_allocated_ = -1;

    // Open file; fails if specified name is empty or file already open
    filename_ = filepath;
    open_mode_ = mode;
    open(filename_);
}

fstream::~fstream() {
    close();
}

void fstream::open(const string &filepath, ios_base::openmode mode) {
    // Check if a file is already open or filepath is invalid
    if (is_open_ || filepath == "")
        return;

    // Initalize permissions (S_..., PROT_..., and MAP_... respectively)
    int open_perms = 0;
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

    // Might need a truncate and/or close call in here somewhere?                                                            TODO


    // Map file to created memory location.
    file_info_ptr_ = reinterpret_cast<char *>(
        mmap(NULL, file_size_, prot_perms, map_perms, file_descriptor_, 0));
    if (file_info_ptr_ == MAP_FAILED) {
        cerr << "fstream::open(): " << strerror(errno) << endl;
        ::close(file_descriptor_);
        exit (-2);
    }

    // Set cursor based on std::ios_base::ate and update EoF status
    cursor_ = 0;
    if (mode & ios_base::ate)
        cursor_ = file_size_;
    if (cursor_ >= file_size_)
        end_of_file_ = true;

    // Indicate that a file is now open.
    is_open_ = true;
}

void fstream::close() {
    if (!is_open_)
        return;
    
    // stuff                                                                                               TODO

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
    // Get next character (update cursor)                                                                         TODO
    return 'A';
}

fstream& fstream::getline(string* line) {
    // Call get until newline character or end of file                                                            TODO
    return *this;
}

fstream& fstream::put(char c) {
    // Add character to end of file, MAY CHANGE FILE SIZE, MAKE SURE TO UPDATE                                     TODO
    return *this;
}

}  // namespace mem_map
