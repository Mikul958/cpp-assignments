// Copyright 2024 CSCE 311
// Modified by Michael Pikula

#ifndef PROJ2_DOMAIN_SOCKET_H_
#define PROJ2_DOMAIN_SOCKET_H_

#include <sys/socket.h>  // Unix header for sockets, using socket
#include <sys/un.h>  // defns for Unix domain sockets, using struct sockaddr_un
#include <unistd.h>  // Unix standard header, using close

#include <cassert>  // using assert
#include <cerrno>  // using errno
#include <cstddef>  // using size_t
#include <cstdlib>  // exit
#include <cstring>  // using strncpy, strerror

#include <string>
#include <vector>
#include <iostream>


class DomainSocket {
 public:
  static const char kEoT = '\004';  // end of transmission; use by default in
                                    //   DomainSocket::Read/Write methods

  static const char kUS = '\037';  // unit separator; provided for child
                                   //   classes

  /// Initializes the socket address struct (::sockaddr_un)
  ///
  /// @param socket_path A null-terminated C-string containing the shared path
  ///                    name for socket.
  /// @param abstract Defaults to abstract socket path names, pass false for
  ///                 non-abstract naming.
  ///
  explicit DomainSocket(const char *socket_path, bool abstract = true);


  bool Init(int domain = AF_UNIX, int type = SOCK_STREAM, int protocol = 0);


  bool Bind();


  bool Listen(std::size_t max_connections = 1) const;


  // Parameter int* is an output parameter returning a file descriptor to a
  // client request socket connection.
  bool Accept(int* client_request_socket_filedesc) const;


  bool Connect() const;


  // Parameter buffer is an output parameter; must be first because all others
  // have defaults
  ::ssize_t Read(std::string* buffer,
                 int socket_file_descriptor = 0,  // 0 uses member socket_fd_
                 std::size_t return_after_bytes = 0,  // 0 reads until EoT char
                 char end_of_transmission = DomainSocket::kEoT) const;


  ::ssize_t Write(const std::string& bytes,
                  int socket_file_descriptor = 0,  // 0 uses member socket_fd_
                  char end_of_transmission = DomainSocket::kEoT) const;


  // 0 uses member socket_fd_
  void Close(int socket_file_descriptor = 0) const;

  // Takes in a vector of strings and builds a message string separated by
  // unit separator '\037' and end of transmission character '\004'
  // Added by Michael Pikula
  std::string BuildMessage(std::vector<std::string> inputs);

  // Takes in a message string with unit separator '\037' and splits it into
  // a vector of strings
  // Added by Michael Pikula
  std::vector<std::string> ParseMessage(std::string message);

 protected:
  int socket_fd_;  // file descriptor for socket this class wraps
  ::sockaddr_un sock_addr_;  // socket address from sys/un.h

  std::string socket_path_;  // std::string stores socket_path (no raw pointers)

 private:
  // This method allows error-checking and messages to be isolated from
  // provided read method
  ::ssize_t Read(int socket_fd, char buffer[], std::size_t buffer_size) const;
};

#endif  // PROJ2_DOMAIN_SOCKET_H_
