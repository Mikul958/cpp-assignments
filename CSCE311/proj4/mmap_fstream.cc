// Copyright 2024 Michael Pikula

#include <proj4/mmap_fstream.h>

#include <iostream>                                                                                  // TODO use some c equivalent for this instead
using std::cout;
using std::cerr;
using std::endl;

namespace mem_map {

fstream::fstream(const string &filepath, ios_base::openmode mode) {
    // All default values, properly set in open() if open successful
    file_descriptor_ = -1;
    cursor_ = -1;
    file_size_ = -1;
    pages_used_ = -1;
    mem_size_ = -1;
    is_open_ = false;
    end_of_file_ = true;        // Unopened file is at EoF apparently
    file_info_ptr_ = nullptr;

    // Open file; fails if specified name is empty or file already open
    filename_ = filepath;
    open_mode_ = mode;
    open(filename_, open_mode_);
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
    if ((mode & ios_base::in) != 0) {
        open_perms = open_perms | S_IRUSR | S_IRGRP;
        prot_perms = prot_perms | PROT_READ;
        map_perms = MAP_PRIVATE;
    }
    if ((mode & ios_base::out) != 0) {
        open_perms = open_perms | S_IWUSR | S_IWGRP;
        prot_perms = prot_perms | PROT_WRITE;
        map_perms = MAP_SHARED;
    }

    // Note: O_CREAT allows file to be created if it doesn't already exist
    // MEMORY MAP OPEN FILE
    file_descriptor_ = ::open(filepath.c_str(), O_CREAT | O_RDWR, open_perms);
    if (file_descriptor_ == -1) {
        cerr << "fstream::open(): " << strerror(errno) << endl;
        return;
    }

    // Get file size using stat.h and file descriptor
    struct stat file_stats;
    ::fstat(file_descriptor_, &file_stats);
    file_size_ = file_stats.st_size;

    // Get number of pages to allocate from file size
    pages_used_ = file_size_ / kPageSize + 1;
    mem_size_ = kPageSize * pages_used_;

    // ALLOCATE FILE MEMORY
    if (::ftruncate(file_descriptor_, mem_size_)) {
        cerr << "fstream::open(): " << strerror(errno) << endl;
        ::close(file_descriptor_);
        return;
    }

    // Map file to created memory location
    file_info_ptr_ = reinterpret_cast<char *>(
        ::mmap(NULL, mem_size_, prot_perms, map_perms, file_descriptor_, 0));
    if (file_info_ptr_ == MAP_FAILED) {
        cerr << "fstream::open(): " << strerror(errno) << endl;
        ::close(file_descriptor_);
        return;
    }

    // Set cursor based on std::ios_base::ate and update EoF status
    cursor_ = 0;
    if ((mode & ios_base::ate) != 0)
        cursor_ = file_size_;
    if (cursor_ < file_size_)
        end_of_file_ = false;

    // Indicate that a file is now open
    is_open_ = true;
}

void fstream::close() {
    // Ensure a file is open to begin with
    if (!is_open_)
        return;
    
    // SAVE TO DISK
    if (::msync(file_info_ptr_, file_size_, MS_SYNC) == -1)
        cerr << "fstream::close(): " << strerror(errno) << endl;

    // Unmap the file from memory
    if (::munmap(file_info_ptr_, kPageSize * pages_used_) == -1)
        cerr << "fstream::close(): " << strerror(errno) << endl;
    
    // Truncate the file that was just written to and close
    if (::ftruncate(file_descriptor_, file_size_))
        cerr << "fstream::close(): " << strerror(errno) << endl;
    ::close(file_descriptor_);

    // Reset all data members to defaults
    // Only is_open_ should matter, but better safe than sorry!
    filename_ = "";
    open_mode_ = ios_base::in | ios_base::out;
    file_descriptor_ = -1;
    cursor_ = -1;
    file_size_ = -1;
    pages_used_ = -1;
    mem_size_ = -1;
    is_open_ = false;
    end_of_file_ = false;
    file_info_ptr_ = nullptr;
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
    // Ensure file is open and cursor isn't at the end
    if (!is_open_ || end_of_file_)
        return '\0';
    
    // READ FROM FILE
    char next = file_info_ptr_[cursor_];
    cursor_++;
    if (cursor_ >= file_size_)
        end_of_file_ = true;
    return next;
}

fstream& fstream::getline(string* line) {
    // Ensure file is open
    if (!is_open_)
        return *this;
    
    // Call get until newline character or EoF, add each result to new string
    string new_line;
    while (!end_of_file_) {
        char next = get();
        if (next == '\n')
            break;
        new_line += next;
    }

    // Update output string and return this fstream
    line->assign(new_line);
    return *this;
}

fstream& fstream::put(char c) {
    // Ensure file is open
    if (!is_open_)
        return *this;
    
    // WRITE TO FILE
    file_info_ptr_[cursor_] = c;
    cursor_++;
    file_size_ = cursor_;
    end_of_file_ = true;

    // Allocate another page of memory if cursor has moved out of bounds
    if (cursor_ >= mem_size_) {
        pages_used_++;
        int new_size = kPageSize * pages_used_;
        
        // Re-map memory with new_size, allowing kernel to move it if needed
        file_info_ptr_ = reinterpret_cast<char *>(
            ::mremap(file_info_ptr_, mem_size_, new_size, MREMAP_MAYMOVE));
        if (file_info_ptr_ == MAP_FAILED) {
            cerr << "fstream::put(): " << strerror(errno) << endl;
            close();
            return *this;
        }
    }

    return *this;
}

}  // namespace mem_map
