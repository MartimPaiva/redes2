#pragma once

#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include <regex.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

class info
{
private:
    int validate(char *arg);
    int get_host_ip();

public:
    char user[256];
    char password[256];
    char host_name[256];
    char host_ip[256];
    char file_path[256];

    int read_commmand(char *url);
    void print();
};