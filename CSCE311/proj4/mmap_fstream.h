// Copyright 2024 Michael Pikula

#ifndef PROJ4_MMAP_FSTREAM_H_
#define PROJ4_MMAP_FSTREAM_H_

#include <fcntl.h>      // O_CREAT, O_RDWR, etc.
#include <sys/mman.h>   // Memory mapping functions
#include <sys/stat.h>   // System info, ex. file sizes
#include <unistd.h>     // UNIX standard header

#include <ios>          // File open permissions
#include <string>
#include <cerrno>
#include <cstdlib>
#include <cstring>

using std::string;
using std::ios_base;


namespace mem_map {

const int kPageSize = 4096;    // 4KB = 1 Page

class fstream {
 public:
  // Creates Memory-mapped file stream obj with file name and open mode
  //
  //   Defaults filepath to "" if none given
  //   Defaults openmode to std::ios_base::in | std::ios_base::out if none given
  //
  //   Result of constructor can be checked with is_open
  //
  //   Must handle any combination of modes
  //     - std::ios_base::ate  (open with cursor at the end of the file)
  //     - std::ios_base::in  (open with read privileges)
  //     - std::ios_base::out  (open with write privileges)
  //
  //  Default parameters to absord above constructors.
  fstream(const string& filepath = "",
          ios_base::openmode mode = ios_base::in | ios_base::out);

  // Destructor, cleans up all information associated with this fstream
  //   Closes any open file in case the user did not.
  ~fstream();

  // Attempts to open file given by name with open mode specified by mode
  //
  //   Defaults openmode to std::ios_base::in | std::ios_base::out if none given
  //
  //   Result can be determined by is_open
  //
  //   Does nothing if file is already open
  //
  //   Must handle any combination of modes
  //     - std::ios_base::ate  (open with cursor at the end of the file)
  //     - std::ios_base::in  (open with read privileges)
  //     - std::ios_base::out  (open with write privileges)
  //
  //   Simplifying assumption: no file will ever be larger than 2^12 bytes
  //   Default openmode parameter to absorb above
  void open(const string& filepath,
            ios_base::openmode mode = ios_base::in | ios_base::out);


  // Attempts to close an open file
  //
  //   Does nothing if file is already open
  //
  //   Simplifying assumption: no file will ever be larger than 2^12 bytes
  //
  void close();


  // Returns file's open state
  //
  bool is_open() const;


  // Returns whether file cursor is currently at end of file; when file is not
  // open, returns true
  bool eof() const;


  // Returns file's current size; may change dynmically due to fstream::put
  //
  std::size_t size() const;


  // Retrieves "next" character from file and updates cursor
  //
  //  Returns '\0' when no further characters exist
  //
  //  This method must not modify a file; only updates cursor position if not
  //  at end of file
  //
  char get();


  // Retrieves "next" line from file, updates cursor, and possibly sets eof flag
  //
  //  This method must not modify a file; only updates cursor position if not
  //  at end of file and possibly modifies eof flag
  //
  fstream& getline(string* line);


  // Writes character at "next" space in the file and updates cursor
  //
  //  This method may increase the size of a file
  //
  fstream& put(char c);

 private:
  // File, cursor, and page information
  string filename_;
  ios_base::openmode open_mode_;
  int file_descriptor_;           // File descriptor for memory location.
  off_t cursor_;                  // Current offset in memory representing file.
  off_t file_size_;               // Tracks current size of the file itself.
  int pages_used_;                // Amount of pages in use.
  int mem_size_;                  // kPageSize * pages_used_
  bool is_open_;
  bool end_of_file_;
  char* file_info_ptr_;           // Buffer holding file information.
};

}  // namespace mem_map

#endif  // PROJ4_MMAP_FSTREAM_H_
