// Copyright 2024 Michael Pikula

#include <proj4/mmap_fstream.h>

namespace mem_map {

fstream::fstream(const string &filepath, ios_base::openmode mode) {
    filename_ = filepath;
    open_mode_ = mode;
    file_descriptor_ = -1;
    cursor_ = -1;
    file_size_ = -1;
    // size_final_ = -1;                                                                       // TODO delete if needed
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
    if (is_open_)
        return;
    
    // stuff                                                                                    TODO

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