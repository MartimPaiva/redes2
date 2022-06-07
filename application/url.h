#ifndef URL_H
#define URL_H

#include "stdio.h"
#include "unistd.h"
#include <regex.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct
{
    char user[256];
    char password[256];
    char host_name[256]; // host name standard size does not surpass
    char host_ip[256];
    char file_path[256];
} application_info;

class url
{
private:
    /* data */
public:
    int validate(char *arg);
    int parse(char *url, application_info *data);
};

#endif