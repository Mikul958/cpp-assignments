# Copyright 2024 Michael Pikula
# Makefile for tests
# Temporary storage location, move to main proj4 folder to use

# Set path and flags
path = ..
flags = -std=c++2a -Wall -pedantic -I $(path) -g

# Set compile/link flags
compile = g++ $(flags) -c -o
link = g++ $(flags) -o

all : test clean

# Link to executable tests
test : test_driver.o mmap_fstream.o
	$(link) $@ $^


# Compile to object code
test_driver.o : test_driver.cc
	$(compile) -c -o $@ $<

mmap_fstream.o : mmap_fstream.cc mmap_fstream.h
	$(compile) -c -o $@ $<

# Clean .o files
clean:
	$(RM) *.o