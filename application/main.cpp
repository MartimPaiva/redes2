#include <fcntl.h>
#include <netdb.h>

#include "debug.h"
#include "info.h"
#include "my_socket.h"

#include <iostream>
using namespace std;
// using std::cout;

#define MAXC 1024
#define FTP_SERVER_PORT 21

int test_input(int argc, char **argv, int *prentended_argument) {
  switch (argc) {
  case 4:
    if (strcmp(argv[0], "valgrind") == 0) {
      cout << "Using valgrind..." << endl;
    } else {
      cout << "You used a invalid input" << endl;
      return -1;
    }
  case 3:
    if (strcmp(argv[1], "download") == 0) {
      // cout << "you used download" << endl;
      FUNCTION_ERROR_CONTROL(validate_command(argv[2]), -1);
      *prentended_argument = 2;
    } else {
      cout << "You used a invalid input" << endl;
      return -1;
    }
    break;
  case 2:
    FUNCTION_ERROR_CONTROL(validate_command(argv[1]), -1);
    *prentended_argument = 1;
    break;
  default:
    cout << "You used a invalid input" << endl;
    return -1;
  }

  return 0;
}

/**
 * @brief Send user info to server to establish_connection
 *
 * @return int
 */
int establish_connection(info *user_info, my_socket *connection_socket) {

  cout << "\n Establishing conncection to server...  " << endl;
  FUNCTION_ERROR_CONTROL(connection_socket->server_connect(user_info, FTP_SERVER_PORT), -1)
  connection_socket->print_info();

  char aux_str[MAXC];
  SOCKET_RECIEVE(connection_socket);
  sprintf(aux_str, "user %s\n\n", user_info->user);
  SOCKET_SEND(connection_socket, aux_str);
  SOCKET_RECIEVE(connection_socket);

  sprintf(aux_str, "PASS %s\n", user_info->password);
  SOCKET_SEND(connection_socket, aux_str);
  SOCKET_RECIEVE(connection_socket);

  return 0;
}

int download_file_imp(info *user_info, my_socket *connection_socket, my_socket *download_socket) {
  char aux_str[MAXC];
  cout << "\nDownloading...  " << endl;

  int download_port = connection_socket->pasv_port();
  FUNCTION_ERROR_CONTROL(download_port, -1)
  download_socket->server_connect(user_info, download_port);

  sprintf(aux_str, "retr %s\n", user_info->file_name);
  SOCKET_SEND(connection_socket, aux_str);
  SOCKET_RECIEVE(connection_socket);

  return download_socket->file_download(user_info);
}

int main(int argc, char **argv) {

  int pretended_argument = 0;
  FUNCTION_ERROR_CONTROL(test_input(argc, argv, &pretended_argument), -1);

  // cout << argv[pretended_argument] << endl;

  info user_info(argv[pretended_argument]);

  FUNCTION_ERROR_CONTROL(user_info.validate(), 0);
  user_info.print();

  my_socket connection_socket("connection_socket"), downlad_socket("downlad_socket");

  FUNCTION_ERROR_CONTROL(establish_connection(&user_info, &connection_socket), 0);
  FUNCTION_ERROR_CONTROL(download_file_imp(&user_info, &connection_socket, &downlad_socket), 0);

  return 0;
}