# Set file path and compiler flags
path = ..
flags = -std=c++2a -Wall -pedantic -I $(path) -g -pthread

compile = g++ $(flags) -c -o
link = g++ $(flags) -o


# Executable server/client
all : server client clean

server : server.o
	$(link) $@ $^

client : client.o calculate.o
	$(link) $@ $^


# Compiled object code
server.o : server.cc server.h shared_mem.h
	$(compile) $@ $<

client.o : client.cc client.h shared_mem.h
	$(compile) $@ $<

calculate.o : calculate.cc calculate.h
	$(compile) $@ $<


# Check style with cpplint
lint : server.cc server.h client.cc client.h calculate.cc calculate.h shared_mem.h
	cpplint --root=$(path) $^


# Remove intermediary .o files
clean:
	$(RM) *.o