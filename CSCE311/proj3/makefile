# Set file path and compiler flags
path = ..
flags = -std=c++2a -Wall -pedantic -I $(path) -g

compile = g++ $(flags) -c -o
compile_thread = g++ $(flags) -c -o
link = g++ $(flags) -o


# Executable server/client
all : server client clean

server : server.o domain_socket.o
	$(link) $@ $^

client : client.o domain_socket.o calculate.o
	$(link) $@ $^


# Compiled object code
server.o : server.cc server.h domain_socket.h
	$(compile) $@ $<

client.o : client.cc client.h domain_socket.h
	$(compile_thread) $@ $<

domain_socket.o : domain_socket.cc domain_socket.h
	$(compile) $@ $<

calculate.o : calculate.cc calculate.h
	$(compile) $@ $<


# Check style with cpplint
lint : server.cc server.h client.cc client.h domain_socket.cc domain_socket.h calculate.cc calculate.h
	cpplint --root=$(path) $^


# Remove intermediary .o files
clean:
	$(RM) *.o