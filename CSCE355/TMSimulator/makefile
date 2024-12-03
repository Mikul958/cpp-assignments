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


# Create tm_simulator executable and clean .o files
all : tm_simulator clean

# Create tm_simulator executable
tm_simulator : turing_machine.o tape.o
	$(link) $@ $^


# Compile turing_machine.cc, ensuring both turing_machine.cc and turing_machine.h exist
turing_machine.o : turing_machine.cc turing_machine.h
	$(compile) $@ $<

# Compile tape.cc, ensuring both tape.cc and tape.h exist
tape.o : tape.cc tape.h
	$(compile) $@ $<


# Remove intermediary .o files
clean:
	$(RM) *.o