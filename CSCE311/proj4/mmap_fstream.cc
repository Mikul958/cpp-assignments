// Copyright 2024 Michael Pikula

#include <proj4/mmap_fstream.h>

namespace mem_map {

fstream::fstream() {

}
fstream::fstream(const string &filename, std::ios_base::openmode mode) {

}

fstream::~fstream() {
    close();
}

void fstream::open(const string &filename) {

}

void fstream::open(const string &filename, std::ios_base::openmode mode) {

}

void fstream::close() {

}

bool fstream::is_open() const {

}

bool fstream::eof() const {

}

std::size_t fstream::size() const {

}

char fstream::get() {

}

fstream& fstream::getline(string* line) {

}

fstream& fstream::put(char c) {

}

}  // namespace mem_map