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

#include "application_info.h"

class url
{
private:
    /* data */
public:
    int validate(char *arg);
    int parse(char *url, application_info *data);
};

#endif