#ifndef MY_SOCKET_H
#define MY_SOCKET_H

#include "libgen.h"
#include "stdio.h"
#include "unistd.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "info.h"

#include <iostream>
using namespace std;

#define SOCKET_RECIEVE(socket_name)                                   \
  {                                                                   \
    if (socket_name->recieve() == -1) {                               \
      cout << "Error in socket recieve SOCKET_RECIEVE(socket_name) "; \
      return -1;                                                      \
    }                                                                 \
  }
#define SOCKET_SEND(socket_name, aux_str)                                         \
  {                                                                               \
    if (!aux_str) {                                                               \
      cout << "You are sending a NULL string" << endl;                            \
    }                                                                             \
    if (socket_name->send_message(aux_str) == -1) {                               \
      cout << "Error sending " << aux_str << " to " << socket_name->name << endl; \
      return -1;                                                                  \
    }                                                                             \
  }

class my_socket {
private:
  FILE *file_pointer;
  char last_reply[512] = {};
  bool connected = false;

public:
  int server_descriptor = -1;
  char name[32];
  void print_info();

  int server_connect(info *user_info, int port);
  int recieve();
  int send_message(char *output);
  int pasv_port();

  int file_download(info *user_info);
  int open_cn();
  ~my_socket();
};

#endif