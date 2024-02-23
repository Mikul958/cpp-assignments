# Flag setup
path = ..
flags = -std=c++2a -Wall -pedantic -I $(path) -g

compile = g++ $(flags) -c -o
link = g++ $(flags) -o


# Executable server/client
server : server.o domain_socket.o
	$(link) $@ $^

client : client.o domain_socket.o
	$(link) $@ $^


# Compiled object code
server.o : server.cc server.h domain_socket.h
	$(compile) $@ $<

client.o : client.cc client.h domain_socket.h
	$(compile) $@ $<

domain_socket.o : domain_socket.cc domain_socket.h
	$(compile) $@ $<

# Clean
clean:
	$(RM) *.o