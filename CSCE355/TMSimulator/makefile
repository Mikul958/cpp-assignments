# $@ is the current target
# $< is the first prerequisite for the target
# $^ is all prerequisites for the target

# Set #include path to parent directory
path = ..
# Use C++20, compile with all warnings, set #include path, compile for GDB
flags = -std=c++2a -Wall -I $(path) -g

# Compile to .o files
compile = g++ $(flags) -c -o
# Link to executable
link = g++ $(flags) -o

# all : calculate clean

# Link main.o and calculator.o to one executable file
#calculate : calculate.o main.o
#	$(link) $@ $^

# Compile main.cc to .o, ensuring it exists
#main.o : main.cc
#	$(compile) $@ $<



# Use for testing tape
tape: tape.o
	$(link) $@ $^


# Compile tape.cc, ensuring both tape.cc and tape.h exist
tape.o : tape.cc tape.h
	$(compile) $@ $<

# Use cpplint on all source files (will not work on Windows)
#lint : calculate.cc calculate.h main.cc
#	cpplint --root=$(path) $^

# Removes intermediary .o files
clean:
	$(RM) *.o