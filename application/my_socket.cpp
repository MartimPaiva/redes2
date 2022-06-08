#include "my_socket.h"

#define MAX_BUFFER_SIZE 256

int my_socket::buff_write(int sockfd, char *buf)
{
    if (write(sockfd, buf, strlen(buf)) < 0)
    {
        perror("write()");
        exit(-1);
    }
    return 0;
}

int my_socket::read_buffer(int sockfd, char *buffer, char *reply)
{
    char server_response[4];
    size_t n = 0;
    ssize_t read;

    FILE *file_pointer = fdopen(sockfd, "r");
    while ((read = getline(&buffer, &n, file_pointer)) != -1)
    {
        if (buffer[3] == ' ')
            break;
    }

    buffer[100 - 1] = '\0';
    strncpy(server_response, buffer, 3);
    server_response[3] = '\0';

    strcpy(reply, buffer);

    printf("Reply message: %s\n", reply);

    return atoi(server_response);
}

int my_socket::server_connect(application_info *data, int port)
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

/**
 * @brief not done
 *
 * @param sockfd
 * @param data
 * @return int
 */
int my_socket::file_download(int sockfd, application_info *data)
{
    char file_name[MAX_BUFFER_SIZE];
    strcpy(file_name, basename(data->file_path));
    char buf[MAX_BUFFER_SIZE];
    int readb = 0;

    int file_descriptor = open(file_name, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);

    printf("FILE FD: %d\n", file_descriptor);
    if (file_descriptor == -1)
    {
        perror("OPEN:");
        return -1;
    }
    while ((readb = read(sockfd, buf, MAX_BUFFER_SIZE)) > 0)
    {
        printf("%s\n", buf);
        if (write(file_descriptor, buf, readb) == -1)
        {
            return -1
        }; // ssize_t write(int fildes, const void *buf, size_t nbytes);
    }

    return close(file_descriptor);
}
