#include "my_socket.h"

#define MAX_BUFFER_SIZE 256
/**
 * @brief not completed
 *
 * @param user_info
 * @param port
 * @return int
 */
int my_socket::server_connect(info *user_info, int port)
{
    struct sockaddr_in server_addr;

    /*server address handling*/
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(user_info->host_ip); /*32 bit Internet address network byte ordered*/
    server_addr.sin_port = htons(port);                          /*server TCP port must be network byte ordered */

    /*open a TCP socket*/
    if ((server_descriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        cout << "Error connection to the server \n";
        return -1;
    }
    if (connect(server_descriptor, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {

        cout << "Error connection to the server\n";
        return -1;
    }
    if (open_cn() == -1)
    {
        cout << "Error connection to the server\n";
        return -1;
    }
    cout << "Socket connect to the server\n";
    open_cn();

    return 1;
}
/**
 * @brief read from socket buffer and returns the server http response number
 *
 * @return int server_response
 */
int my_socket::recieve()
{
    open_cn();

    char server_response[4] = {};

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, file_pointer)) != -1)
    {
        if (line[3] == ' ')
        {
            strncpy(server_response, line, 3);
            server_response[3] = '\0';

            free(line);
            break;
        }
    }
    fclose(file_pointer);

    int response_code = atoi(server_response);

    if (response_code > 399)
    {
        close_cn();
        return -1;
    }
}
/**
 * @brief Send message to the server throw the socket
 *
 * @param input
 * @return -1: error; 0: sucess;
 */
int my_socket::send_message(char *input)
{

    if (write(server_descriptor, input, strlen(input)) == -1)
    {
        close_cn();
        return -1;
    }
    return 0;
}

/**
 * @brief done, not tested
 *
 * @param server_descriptor
 * @param data
 * @return int
 */
int my_socket::file_download(info *user_info)
{
    int downloaded_file_descriptor = open(basename(user_info->file_path), O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);

    if (downloaded_file_descriptor == -1)
    {
        close_cn();
        return -1;
    }

    int write_success = 0;

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, this->file_pointer)) != -1)
    {
        if (write(downloaded_file_descriptor, line, len) == -1)
        {
            write_success = -1;
            close_cn();
            break;
        };
    }
    free(line);
    close(downloaded_file_descriptor);

    return write_success;
}
/**
 * @brief não está acabado
 *
 * @return int
 */
int my_socket::pasv_port()
{
    if (!last_reply)
    {
        return -1;
    }

    cout << last_reply;

    char res[256];
    char first_byte[4];
    char second_byte[4];

    strtok(last_reply, "(");
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
/**
 * @brief close the socket connection
 *
 */
void my_socket::close_cn()
{
    close(server_descriptor);
}

/**
 * @brief open the socket connection
 *
 */
int my_socket::open_cn()
{
    file_pointer = fdopen(server_descriptor, "r");

    if (file_pointer == NULL)
    {
        cout << "Error opening connection \n";
        return -1;
    }

    return 0;
}
