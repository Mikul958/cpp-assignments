// YOUR DOCUMENTATION GOES HERE
//
//
#include <ios>
#include <string>

using std::string;

namespace mem_map {

class fstream {
 public:
  // Creates Memory-mapped file stream obj without file name; file name must be
  //   specified in call to Open.
  //
  //   You may use default parameter values to allow following constructors to
  //   absorb this one
  //
  fstream();


  // Creates Memory-mapped file stream obj with file name
  //
  //   Open mode is std::ios_base::in | std::ios_base:: out by default.
  //
  //   Result of constructor can be determined by is_open
  //
  //   You may use default parameter values to allow following constructor to
  //   absorb this one
  //
  // Unnecessary, constructor below has default param
  // explicit fstream(const string& fname);


  // Creates Memory-mapped file stream obj with file name and open mode
  // Openmode defaults to in/out permissions
  //
  //   Result of constructor can be checked with is_open
  //
  //   Must handle any combination of modes
  //     - std::ios_base::ate  (open with cursor at the end of the file)
  //     - std::ios_base::in  (open with read privileges)
  //     - std::ios_base::out  (open with write privileges)
  // 
  fstream(const string& fname,
         std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);

  // Destructor
  ~fstream();

  // Attempts to open file given by file_name
  //
  //   Open mode is std::ios_base::in | std::ios_base::out by default
  //
  //   Result can be determined by is_open
  //
  //   Does nothing if file is already open
  //
  //   You may use default parameter values to overload following method to
  //   absorb this one
  //
  //   Simplifying assumption: no file will ever be larger than 2^12 bytes
  //
  void open(const string& fname);


  // Attempts to open file given by name with open mode specified by mode
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
  //
  void open(const string& fname, std::ios_base::openmode mode);


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
  // File information
  off_t cursor_ = -1;
  off_t size_ = -1;
  off_t size_final_ = -1;  // Updated when saving
  string filepath_;
  char* buffer_ptr_ = nullptr;
  int file_descriptor_ = -1;

  // Cursor controls
  inline void IncrementCursor(off_t amount) {
    cursor_ += amount;
    if (cursor_ > size_final_)
      size_final_ = cursor_;
  }
  inline void SetCursor(off_t location) {
    cursor_ = location;
    if (cursor_ > size_final_)
      size_final_ = cursor_;
  }
};

}  // namespace mem_map
