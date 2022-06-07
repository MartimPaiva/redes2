#include "url.h"

int url::parse(char *url, application_info *data)
{
    int index = 6; // skip ftp://

    char *found_at = strstr(url, "@");

    if (found_at != NULL)
    {
        // user password
        // char *strncpy(char *restrict dest, const char *restrict src, size_t n);
        // index
        while (url[index] != ':')
            index++;
        strncpy(data->user, url + 6, index - 6);
        data->user[index - 6] = '\0';
        int offset = ++index;
        while (url[index] != '@')
            index++;
        strncpy(data->password, url + offset, index - offset);
        data->password[index - offset] = '\0';
        index++;
    }
    else
    {
        strcpy(data->user, "anonymous");
        strcpy(data->password, "anonymous");
    }

    // host
    int host_delim_index = index;

    while (url[host_delim_index] != '/')
        ++host_delim_index;

    strncpy(data->host_name, url + index, host_delim_index - index);

    data->host_name[host_delim_index - index] = '\0';

    index = host_delim_index + 1;

    // filepath
    int path_delim_index = index;

    while (url[path_delim_index] != '\0')
        ++path_delim_index;

    strncpy(data->file_path, url + index, path_delim_index - index);

    data->file_path[path_delim_index - index] = '\0';

    printf("User: %s\n", data->user);
    printf("Password: %s\n", data->password);
    printf("hostName: %s\n", data->host_name);
    printf("filpath: %s\n", data->file_path);

    return 0;
}

int url::validate(char *arg)
{
    regex_t regex_exp;
    if (regcomp(&regex_exp, "^ftp://([a-z0-9]+:[a-z0-9]+@)?([\\.a-z0-9-]+)/([\\./a-z0-9-]+)$", REG_EXTENDED | REG_ICASE))
    {
        fprintf(stderr, "Failed validation proccess.");
        exit(1);
    }
    int res = regexec(&regex_exp, arg, 0, NULL, 0);
    regfree(&regex_exp);
    return res;
}
