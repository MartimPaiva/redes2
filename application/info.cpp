#include "info.h"

/**
 * @brief precisa de ser revisto
 *
 *
 * @param command
 * @return int
 */
int info::read_commmand(char *command)
{
    if (validate(command) == -1)
    {
        return -1;
    }

    int index = 6; // skip ftp://

    char *found_at = strstr(command, "@");

    if (found_at != NULL)
    {
        // user password
        // char *strncpy(char *restrict dest, const char *restrict src, size_t n);
        // index
        while (command[index] != ':')
            index++;
        strncpy(user, command + 6, index - 6);
        user[index - 6] = '\0';
        int offset = ++index;
        while (command[index] != '@')
            index++;
        strncpy(password, command + offset, index - offset);
        password[index - offset] = '\0';
        index++;
    }
    else
    {
        strcpy(user, "anonymous");
        strcpy(password, "anonymous");
    }

    // host
    int host_delim_index = index;

    while (command[host_delim_index] != '/')
        ++host_delim_index;

    strncpy(host_name, command + index, host_delim_index - index);

    host_name[host_delim_index - index] = '\0';

    index = host_delim_index + 1;

    // filepath
    int path_delim_index = index;

    while (command[path_delim_index] != '\0')
        ++path_delim_index;

    strncpy(file_path, command + index, path_delim_index - index);

    file_path[path_delim_index - index] = '\0';

    return get_host_ip();
}
/**
 * @brief get the host ip from user host name
 *
 * @return int
 */
int info::get_host_ip()
{
    struct hostent *h;

    if ((h = gethostbyname(host_name)) == NULL)
    {
        return -1;
    }
    strcpy(host_ip, inet_ntoa(*((struct in_addr *)h->h_addr)));

    return 0;
}

int info::validate(char *command)
{
    regex_t reg_expression;
    if (regcomp(&reg_expression, "^ftp://([a-z0-9]+:[a-z0-9]+@)?([\\.a-z0-9-]+)/([\\./a-z0-9-]+)$", REG_EXTENDED | REG_ICASE))
    {
        return -1;
    }
    int res = regexec(&reg_expression, command, 0, NULL, 0);
    regfree(&reg_expression);
    return res;
}

void info::print()
{
    printf("User: %s\n", user);
    printf("Password: %s\n", password);
    printf("Host name: %s\n", host_name);
    printf("File path: %s\n", file_path);
}