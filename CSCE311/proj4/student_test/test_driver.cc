// Copyright 2024 Michael Pikula
//
// Large-scale personal test for Project 4 using custom fstream object
// Not submitted with rest of project, must be moved into parent proj4
// directory with makefile to work
//

#include <proj4/mmap_fstream.h>

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;
using std::cout;
using std::endl;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "\n\tUsage : ./test <read_path> <write_path> <line_to_append>\n"
             << endl;
        return -1;
    }
    
    // Initialize fstream, open first file (path arg1) with default permissions
    string path = argv[1];
    mem_map::fstream my_fstream(path);
    vector<string> file_lines;

    // Read all lines in first file and add to vector
    while (!my_fstream.eof()) {
        string new_line;
        my_fstream.getline(&new_line);
        file_lines.push_back(new_line);
    }

    // Close first file
    my_fstream.close();

    // Open new file with path arg2 with default permissions
    path = argv[2];
    my_fstream.open(path);

    // Copy read contents to new file (overwrite) and close
    for (string s : file_lines) {
        for (char c : s)
            my_fstream.put(c);
        my_fstream.put('\n');
    }
    my_fstream.close();

    // Open new file again, this time with cursor at the end
    ios_base::openmode mode = ios_base::in | ios_base::out | ios_base::ate;
    my_fstream.open(path, mode);

    // Append line at arg3 to new file
    string message = argv[3];
    for (char c : message)
        my_fstream.put(c);
    my_fstream.put('\n');
    
    return 0;
}