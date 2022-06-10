// ./app ftp:// netlab1.fe.up.pt/pub.txt
// ./app ftp://rcom:rcom@netlab1.fe.up.pt/pipe.txt

#include <netdb.h>
#include <fcntl.h>

#include "info.h"
#include "my_socket.h"

#include <iostream>
using namespace std;
// using std::cout;

#define MAXC 512
#define PORT 6000
#define SERVER_ADDR "192.168.28.96"

// user, password, host, url path
// ftp://[<user>:<password>@]<host>/<url-path>

/**
 * @brief Send user info to server to establish_connection
 *
 * @return int
 */
int establish_connection(info *user_info, my_socket *connection_socket)
{

    connection_socket->server_connect(user_info, PORT);

    char aux_str[MAXC];

    sprintf(aux_str, "user %s \n", user_info->user);
    SOCKET_SEND(connection_socket, aux_str);
    SOCKET_RECIEVE(connection_socket);

    sprintf(aux_str, "password %s \n", user_info->password);
    SOCKET_SEND(connection_socket, aux_str);
    SOCKET_RECIEVE(connection_socket);

    return 0;
}

int close_connection(my_socket *socket)
{
    char aux_str[MAXC];

    sprintf(aux_str, "quit\n");
    SOCKET_SEND(socket, aux_str);

    socket->close_cn();

    return 0;
}

int download_file_imp(info *user_info, my_socket *connection_socket, my_socket *download_socket)
{
    char aux_str[MAXC];

    sprintf(aux_str, "pasv\n");
    SOCKET_SEND(connection_socket, aux_str);
    SOCKET_RECIEVE(connection_socket);

    int download_port = connection_socket->pasv_port();
    download_socket->server_connect(user_info, download_port);

    sprintf(aux_str, "retr %s\n", user_info->file_path);
    SOCKET_SEND(connection_socket, aux_str);
    SOCKET_RECIEVE(connection_socket);

    return download_socket->file_download(user_info);
}

int main(int argc, char **argv)
{
    info user_info;
    if (argc != 2)
    {
        cout << "USE: download ftp://[<user>:<password>@]<host>/<url-path>\n";
        return -1;
    }

    if (user_info.read_commmand(argv[1]) == -1)
    {
        return -1;
    }

    user_info.print();

    my_socket connection_socket, downlad_socket;
    if (establish_connection(&user_info, &connection_socket) == -1)
    {
        return -1;
    }

    if (download_file_imp(&user_info, &connection_socket, &downlad_socket) == -1)
    {
        return -1;
    }

    if (close_connection(&downlad_socket) == -1)
    {
        cout << "error";
    }

    if (close_connection(&connection_socket) == -1)
    {
        return -1;
    }

    // int port = pasv_port(socket1.last_reply); // get port from pasv cmd
    // printf("Port %d\n", port);

    // // socket.server_connect to host
    // int sockfd2 = socket2.server_connect(&data, port);

    // // see what type of verification we can do in term_b message reply "]"

    // bzero(buf, 256);
    // strcpy(buf, "retr ");
    // strcat(buf, data.file_path);
    // strcat(buf, "\n");
    // socket1.send_message(sockfd1, buf);
    // printf("wrote to socket:%s\n", buf);

    // r =  socket1.recieve(sockfd1, reply, buffer); // socket.read_buffer
    // if (r >= 400)
    // {
    //     fprintf(stderr, "Connection error with return code %d\n", r);
    //     close(sockfd1);
    //     close(sockfd2);
    //     exit(1);
    // }

    // // download file on sockfd2
    // socket2.file_download(sockfd2, &data);

    // r =  socket1.recieve(sockfd1, reply, buffer); // socket.read_buffer
    // if (r >= 400)
    // {
    //     fprintf(stderr, "Connection error with return code %d\n", r);
    //     close(sockfd1);
    //     close(sockfd2);
    //     exit(1);
    // }
    // // close sockfd socket
    // bzero(buf, 256);
    // strcpy(buf, "quit");
    // strcat(buf, "\n");
    // socket1.send_message(sockfd1, buf);
    // printf("wrote to socket:%s\n", buf);

    // r =  socket1.recieve(sockfd1, reply, buffer); // socket.read_buffer
    // if (r >= 400)
    // {
    //     fprintf(stderr, "Connection error with return code %d\n", r);
    //     close(sockfd1);
    //     close(sockfd2);
    //     exit(1);
    // }

    // close(sockfd1);
    // close(sockfd2);
    return 0;
}