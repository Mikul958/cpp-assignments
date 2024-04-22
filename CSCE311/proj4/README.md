# Program 4 - Implement Custom fstream Interface

**mmap_fstream.cc / mmap_fstream.h:**  
Provides functionality for the fstream interface, including the ability to:
- Open files (creating a new file if it doesn't exist)
- Read from and write to open files
- Save modified files to disk

Implementations of Major Steps in mmap_fstream.cc:
a) Memory mapped:
    fstream::open() - line 53
b) Size of file in memory set:
    fstream::open() - line 70
c) File read from memory map:
    fstream::get() - line 148
d) File in memory map written to:
    fstream::put() - line 180
e) File saved to disk:
    fstream::close() - line 102
