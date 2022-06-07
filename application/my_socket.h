#ifndef SOCKET_FUN_H
#define SOCKET_FUN_H

#include "stdio.h"
#include "unistd.h"
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class my_socket
{
private:
    /* data */
public:
    int connect(application_info *data, int port);
    int read(int sockfd, char *buf, char *buffer);
    int write(int sockfd, char *buf);
};

#endif