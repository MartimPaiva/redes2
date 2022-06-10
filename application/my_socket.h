#ifndef MY_SOCKET_H
#define MY_SOCKET_H

#include <sys/socket.h>
#include "stdio.h"
#include "unistd.h"
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "info.h"

#include <iostream>
using namespace std;

#define SOCKET_RECIEVE(socket_name)       \
    {                                     \
        if (socket_name->recieve() == -1) \
        {                                 \
            return -1;                    \
        }                                 \
    }
#define SOCKET_SEND(socket_name, aux_str)             \
    {                                                 \
        if (socket_name->send_message(aux_str) == -1) \
        {                                             \
            return -1;                                \
        }                                             \
    }

class my_socket
{
private:
    FILE *file_pointer;
    char *last_reply;

public:
    int server_descriptor;

    int server_connect(info *user_info, int port);
    int recieve();
    int send_message(char *output);
    int pasv_port();

    int file_download(info *user_info);
    int open_cn();
    void close_cn();
};

#endif