// ./app ftp:// netlab1.fe.up.pt/pub.txt
// ./app ftp://rcom:rcom@netlab1.fe.up.pt/pipe.txt

#include <netdb.h>
#include <fcntl.h>

#include "url.h"
#include "my_socket.h"
#include "application_info.h"

#define SERVER_PORT 6000
#define SERVER_ADDR "192.168.28.96"

// user, password, host, url path

int getHostName(application_info *data)
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
/* ftp://[<user>:<password>@]<host>/<url-path>
Ex  ftp://netlab1.fe.up.pt/pub.txt
Ex  ftp://rcom:rcom@netlab1.fe.up.pt/pipe.txt
*/

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

int main(int argc, char **argv)
{
    my_socket socket;
    url URL;
    if (argc != 2)
    {
        fprintf(stderr, "USE: download ftp://[<user>:<password>@]<host>/<url-path>\n");
        return -1;
    }

    // URL.validate url
    if (URL.validate(argv[1]) != 0)
    {
        fprintf(stderr, "Invalid url\n");
        return -1;
    }

    // URL.parse url
    application_info data;
    URL.parse(argv[1], &data);

    // hostname
    getHostName(&data);

    // socket.server_connect to host
    int sockfd = socket.server_connect(&data, 21);

    // user credentials
    char buf[256];
    char buffer[256];
    char reply[256];
    int r = 0;

    r = socket.read_buffer(sockfd, reply, buffer); // socket.read_buffer
    if (r >= 400)
    {
        fprintf(stderr, "Connection error with return code %d\n", r);
        close(sockfd);
        exit(1);
    }

    strcpy(buf, "user "); // user write
    strcat(buf, data.user);
    strcat(buf, "\n");
    socket.buff_write(sockfd, buf);
    printf("wrote to socket:%s\n", buf);

    r = socket.read_buffer(sockfd, reply, buffer); // socket.read_buffer
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
    socket.buff_write(sockfd, buf);
    printf("wrote to socket:%s\n", buf);

    r = socket.read_buffer(sockfd, reply, buffer); // socket.read_buffer
    if (r >= 400)
    {
        fprintf(stderr, "Connection error with return code %d\n", r);
        close(sockfd);
        exit(1);
    }
    // pasv mode
    bzero(buf, 256);
    strcpy(buf, "pasv\n"); // pasv write
    socket.buff_write(sockfd, buf);
    printf("wrote to socket:%s\n", buf);

    r = socket.read_buffer(sockfd, reply, buffer); // socket.read_buffer
    if (r >= 400)
    {
        fprintf(stderr, "Connection error with return code %d\n", r);
        close(sockfd);
        exit(1);
    }

    int port = ipParser(reply); // get port from pasv cmd
    printf("Port %d\n", port);

    // socket.server_connect to host
    int sockfd2 = socket.server_connect(&data, port);

    // see what type of verification we can do in term_b message reply "]"

    bzero(buf, 256);
    strcpy(buf, "retr ");
    strcat(buf, data.file_path);
    strcat(buf, "\n");
    socket.buff_write(sockfd, buf);
    printf("wrote to socket:%s\n", buf);

    r = socket.read_buffer(sockfd, reply, buffer); // socket.read_buffer
    if (r >= 400)
    {
        fprintf(stderr, "Connection error with return code %d\n", r);
        close(sockfd);
        close(sockfd2);
        exit(1);
    }

    // download file on sockfd2
    socket.file_download(sockfd2, &data);

    r = socket.read_buffer(sockfd, reply, buffer); // socket.read_buffer
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
    socket.buff_write(sockfd, buf);
    printf("wrote to socket:%s\n", buf);

    r = socket.read_buffer(sockfd, reply, buffer); // socket.read_buffer
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