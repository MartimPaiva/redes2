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

#define SERVER_PORT 6000
#define SERVER_ADDR "192.168.28.96"

// user, password, host, url path

typedef struct
{
    // char url_path[256];
    char user[256];
    char password[256];
    char host_name[256]; // host name standard size does not surpass
    char host_ip[256];
    char file_path[256];
} app_t;

int url_validator(char *arg)
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

int getHostName(app_t *data)
{
    struct hostent *h;

    if ((h = gethostbyname(data->host_name)) == NULL)
    {
        herror("gethostbyname()");
        exit(-1);
    }

    strcpy(data->host_ip, inet_ntoa(*((struct in_addr *)h->h_addr)));

    printf("Host name  : %s\n", h->h_name);
    printf("IP Address : %s\n", data->host_ip);

    return 0;
}

int connectToSocket(app_t *data, int port)
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

/* ftp://[<user>:<password>@]<host>/<url-path>
Ex  ftp://netlab1.fe.up.pt/pub.txt
Ex  ftp://rcom:rcom@netlab1.fe.up.pt/pipe.txt
*/
int urlParser(char *url, app_t *data)
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

int writeToSocket(int sockfd, char *buf)
{
    if (write(sockfd, buf, strlen(buf)) < 0)
    {
        perror("write()");
        exit(-1);
    }
    return 0;
}

int readFromSocket(int sockfd, char *buf, char *buffer)
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

int ipParser(char *reply)
{
    char res[256];
    char first_byte[4];
    char second_byte[4];

    strtok(reply, "(");
    strcpy(res, strtok(NULL, "("));
    strcpy(res, strtok(res, ")"));

    strtok(res, ",");
    for (int i = 0; i < 3; i++)
    {
        strtok(NULL, ","); // ignore until last two numbers
    }

    strcpy(first_byte, strtok(NULL, ","));
    strcpy(second_byte, strtok(NULL, ","));
    return atoi(first_byte) * 256 + atoi(second_byte); // port number
}

int downloadFile(int sockfd, app_t *data)
{
    printf("Writing to file\n");

    char file_name[256];
    strcpy(file_name, basename(data->file_path));
    char read_buf[256];
    int readb = 0;

    int filefd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);

    printf("FILE FD: %d\n", filefd);
    if (filefd == -1)
    {
        perror("OPEN:");
        return -1;
    }
    while ((readb = read(sockfd, read_buf, 256)) > 0)
    {
        printf("%s\n", read_buf);
        write(filefd, read_buf, readb);
    }

    return close(filefd);
}

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        fprintf(stderr, "Maner of use: download ftp://[<user>:<password>@]<host>/<url-path>\n");
        return -1;
    }

    // validate url
    if (url_validator(argv[1]) != 0)
    {
        fprintf(stderr, "Invalid url\nManer of use: download ftp://[<user>:<password>@]<host>/<url-path>\n");
        return -1;
    }

    // parse url
    app_t data;
    urlParser(argv[1], &data);

    // hostname
    getHostName(&data);

    // connect to host
    int sockfd = connectToSocket(&data, 21);

    // user credentials
    char buf[256];
    char buffer[256];
    char reply[256];
    int r = 0;

    r = readFromSocket(sockfd, reply, buffer); // read
    if (r >= 400)
    {
        fprintf(stderr, "Connection error with return code %d\n", r);
        close(sockfd);
        exit(1);
    }

    strcpy(buf, "user "); // user write
    strcat(buf, data.user);
    strcat(buf, "\n");
    writeToSocket(sockfd, buf);
    printf("wrote to socket:%s\n", buf);

    r = readFromSocket(sockfd, reply, buffer); // read
    if (r >= 400)
    {
        fprintf(stderr, "Connection error with return code %d\n", r);
        close(sockfd);
        exit(1);
    }

    bzero(buf, 256);
    strcpy(buf, "pass "); // password write
    strcat(buf, data.password);
    strcat(buf, "\n");
    writeToSocket(sockfd, buf);
    printf("wrote to socket:%s\n", buf);

    r = readFromSocket(sockfd, reply, buffer); // read
    if (r >= 400)
    {
        fprintf(stderr, "Connection error with return code %d\n", r);
        close(sockfd);
        exit(1);
    }
    // pasv mode
    bzero(buf, 256);
    strcpy(buf, "pasv\n"); // pasv write
    writeToSocket(sockfd, buf);
    printf("wrote to socket:%s\n", buf);

    r = readFromSocket(sockfd, reply, buffer); // read
    if (r >= 400)
    {
        fprintf(stderr, "Connection error with return code %d\n", r);
        close(sockfd);
        exit(1);
    }

    int port = ipParser(reply); // get port from pasv cmd
    printf("Port %d\n", port);

    // connect to host
    int sockfd2 = connectToSocket(&data, port);

    // see what type of verification we can do in term_b message reply "]"

    bzero(buf, 256);
    strcpy(buf, "retr ");
    strcat(buf, data.file_path);
    strcat(buf, "\n");
    writeToSocket(sockfd, buf);
    printf("wrote to socket:%s\n", buf);

    r = readFromSocket(sockfd, reply, buffer); // read
    if (r >= 400)
    {
        fprintf(stderr, "Connection error with return code %d\n", r);
        close(sockfd);
        close(sockfd2);
        exit(1);
    }

    // download file on sockfd2
    downloadFile(sockfd2, &data);

    r = readFromSocket(sockfd, reply, buffer); // read
    if (r >= 400)
    {
        fprintf(stderr, "Connection error with return code %d\n", r);
        close(sockfd);
        close(sockfd2);
        exit(1);
    }
    // close sockfd socket
    bzero(buf, 256);
    strcpy(buf, "quit");
    strcat(buf, "\n");
    writeToSocket(sockfd, buf);
    printf("wrote to socket:%s\n", buf);

    r = readFromSocket(sockfd, reply, buffer); // read
    if (r >= 400)
    {
        fprintf(stderr, "Connection error with return code %d\n", r);
        close(sockfd);
        close(sockfd2);
        exit(1);
    }

    close(sockfd);
    close(sockfd2);
    return 0;
}