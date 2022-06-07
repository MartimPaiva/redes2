#include "my_socket.h"

int my_socket::write(int sockfd, char *buf)
{
    if (write(sockfd, buf, strlen(buf)) < 0)
    {
        perror("write()");
        exit(-1);
    }
    return 0;
}

int my_socket::read(int sockfd, char *buf, char *buffer)
{
    char code[4];
    size_t n = 0;
    ssize_t read;

    FILE *fp = fdopen(sockfd, "r");
    while ((read = getline(&buffer, &n, fp)) != -1)
    {
        if (buffer[3] == ' ')
            break;
    }

    buffer[100 - 1] = '\0';
    strncpy(code, buffer, 3);
    code[4] = '\0';

    strcpy(buf, buffer);

    printf("Reply message: %s\n", buf);

    return atoi(code);
}

int my_socket::connect(application_info *data, int port)
{
    int sockfd;
    struct sockaddr_in server_addr;

    /*server address handling*/
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(data->host_ip); /*32 bit Internet address network byte ordered*/
    server_addr.sin_port = htons(port);                     /*server TCP port must be network byte ordered */

    /*open a TCP socket*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket()");
        exit(-1);
    }
    // printf("socket ready\n");
    /*connect to the server*/
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect()");
        exit(-1);
    }
    return sockfd;
}
