#ifndef URL_H
#define URL_H

#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#include "application_info.h"

class url
{
public:
    int validate(char *arg);
    int parse(char *url, application_info *data);
};

#endif