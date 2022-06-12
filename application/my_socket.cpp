#include "my_socket.h"

#define MAX_BUFFER_SIZE 256

/**
 * @brief close the socket connection
 *
 */
my_socket::~my_socket() {
  if (connected == false)
    return;

  if (file_descriptor != -1) {
    char aux_str[] = "quit\n";
    send_message(aux_str);

    fclose(file_pointer);
    close(file_descriptor);
  }
}

/**
 * @brief not completed
 *
 * @param user_info
 * @param port
 * @return int
 */
int my_socket::server_connect(info *user_info, int port) {
  struct sockaddr_in server_addr;

  bzero((char *)&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(user_info->host_ip); /*32 bit Internet address network byte ordered*/
  server_addr.sin_port = htons(port);                          /*server TCP port must be network byte ordered */

  if ((file_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket()");
    return -1;
  }
  if (connect(file_descriptor, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("connect()");
    return -1;
  }
  // FUNCTION_ERROR_CONTROL((server_descriptor = socket(AF_INET, SOCK_STREAM, 0)), -1)
  // FUNCTION_ERROR_CONTROL(connect(server_descriptor, (struct sockaddr *)&server_addr, sizeof(server_addr)), -1)
  // FUNCTION_ERROR_CONTROL(open_cn(), -1)
  FUNCTION_ERROR_CONTROL(send_message("wake up server"), -1)

  return 0;
}
/**
 * @brief read from socket buffer and returns the server http response number
 *
 * @return int server_response
 */
int my_socket::recieve() {
  char server_response[4] = {};

  char *line = NULL;
  size_t len = 0;

  ssize_t read;
  file_pointer = fdopen(file_descriptor, "r");
  // cout << "---begin received--- \n";
  while ((read = getline(&line, &len, file_pointer)) != -1) {
    cout << line << "---received " << endl;
    if (line[3] == ' ') {
      strncpy(server_response, line, 3);
      server_response[3] = '\0';

      free(line);
      break;
    }
    fclose(file_pointer);
  }
  // cout << "---end received---\n ";

  int response_code = atoi(server_response);

  if (response_code > 399) {
    cout << "Status code error \n";
    return -1;
  }
  return 0;
}
/**
 * @brief Send message to the server throw the socket
 *
 * @param input
 * @return -1: error; 0: sucess;
 */
int my_socket::send_message(char *input) {

  if (!input) {
    cout << "NULL input at send_message(char *input)" << endl;
    return -1;
  }
  if (strlen(input) < 1) {
    cout << "Empty message at send_message(char *input) " << endl;
    return -1;
  }

  if (write(this->file_descriptor, input, strlen(input)) < 0) {
    cout << "\nError writing input to server ( write(server_descriptor, input, strlen(input) == -1)" << endl;
    cout << "Probably invalid server_descriptor" << endl;
    cout << "Server descriptor: " << file_descriptor << endl;
    return -1;
  }
  cout << "message send: '" << input << "'\n";
  return 0;
}
/**
 * @brief done, not tested
 *
 * @param server_descriptor
 * @param data
 * @return int
 */
int my_socket::file_download(info *user_info) {
  int downloaded_file_descriptor = open(basename(user_info->file_name), O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);

  if (downloaded_file_descriptor == -1) {
    return -1;
  }

  int write_success = 0;

  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  while ((read = getline(&line, &len, this->file_pointer)) != -1) {
    if (write(downloaded_file_descriptor, line, len) == -1) {
      write_success = -1;
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
int my_socket::pasv_port() {
  if (send_message("pasv\n") == -1) {
    cout << "Error sending pasv\\n to server" << endl;
    return -1;
  }
  recieve();
  if (!last_reply) {
    cout << "NULL last reply" << endl;
    return -1;
  }
  if (strlen(last_reply) < 6) {
    cout << "Invalid last reply" << endl;
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
  for (int i = 0; i < 3; i++) {
    strtok(NULL, ","); // ignore until last two numbers
  }

  strcpy(first_byte, strtok(NULL, ","));
  strcpy(second_byte, strtok(NULL, ","));
  return atoi(first_byte) * 256 + atoi(second_byte); // port number
}
/**
 * @brief Creates a new socket  with the specified name.
 * @param _name The name of the socket.
 *
 */
my_socket::my_socket(const char *_name) {
  // TEST_C_LIKE_STRING(_name, ;)
  strcpy(name, _name);
}

/**
 * @brief open the socket connection
 *
 */
int my_socket::open_cn() {
  file_pointer = fdopen(file_descriptor, "r");

  if (file_pointer == NULL) {
    cout << "Error opening connection \n";
    return -1;
  }

  return 0;
}

void my_socket::print_info() {
  cout << "Socket" << name << "info" << endl;
  cout << "Server descriptor: " << file_descriptor << endl;
}