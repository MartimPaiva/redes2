#ifndef MY_SOCKET_H
#define MY_SOCKET_H

#include <sys/socket.h>
#include "stdio.h"
#include "unistd.h"
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "application_info.h"

class my_socket
{
public:
    int server_connect(application_info *data, int port);
    int read(int sockfd, char *buf, char *buffer);
    int buff_write(int sockfd, char *buf);
};

#endif