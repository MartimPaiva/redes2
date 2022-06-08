#ifndef MY_SOCKET_H
#define MY_SOCKET_H

#include <sys/socket.h>
#include "stdio.h"
#include "unistd.h"
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#include "application_info.h"

class my_socket
{
public:
    int server_connect(application_info *data, int port);
    int read_buffer(int sockfd, char *buf, char *buffer);
    int buff_write(int sockfd, char *buf);
    int file_download(int sockfd, application_info *data);
};

#endif