// Copyright 2024 Michael Pikula

#include <proj4/mmap_fstream.h>

namespace mem_map {

fstream::fstream(const string &filename, std::ios_base::openmode mode) {
    filename_ = filename;
    open_mode_ = mode;
    file_descriptor_ = -1;
    cursor_ = -1;
    size_ = -1;
    size_final_ = -1;                                                                       // TODO delete if needed
    file_open_ = false;
    end_of_file_ = false;
    buffer_ptr_ = nullptr;

    // Open file if name was given
    if (filename_ != "")
        open(filename_);
}

fstream::~fstream() {
    close();
}

void fstream::open(const string &filename, std::ios_base::openmode mode) {

}

void fstream::close() {

}

// Set to true on file open, set to false on file close
bool fstream::is_open() const {
    return file_open_;
}

// Set to true if cursor is >= file size
bool fstream::eof() const {
    return end_of_file_;
}

std::size_t fstream::size() const {
    return size_;
}

char fstream::get() {
    // Get next character (update cursor)
}

fstream& fstream::getline(string* line) {
    // Call get until newline character or end of file
}

fstream& fstream::put(char c) {
    // Add character to end of file (not quite sure how?)
}

}  // namespace mem_map