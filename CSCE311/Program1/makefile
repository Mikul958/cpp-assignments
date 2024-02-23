# Makefile for Program1 and future reference.

# $@ is the current target
# $< is the first prerequisite for the target
# $^ is all prerequisites for the target

# Set #include path to parent directory
path = ..
# Use C++20, compile with all warnings, set #include path, compile for GDB
flags = -std=c++2a -Wall -I $(path)

# Compilation to .o files
compile = g++ $(flags) -c -o
# Link to executable
link = g++ $(flags) -o

# Link main.o and calculator.o to one executable file
calculate : main.o calculate.o
	$(link) $@ $^

# Compile main.cc to .o, ensuring it exists
main.o : main.cc
	$(compile) $@ $<

# Compile calculator.cc, ensuring both calculator.cc and calculator.h exist
calculate.o : calculate.cc calculate.h
	$(compile) $@ $<

# Use cpplint on all source files (will not work on Windows)
lint : calculate.cc calculate.h main.cc
	cpplint --root=$(path) $^

# Removes intermediary .o files
clean:
	$(RM) *.o