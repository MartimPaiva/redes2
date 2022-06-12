#include "info.h"

/**
 * @brief Validate and parse the configuration for the ftp connection;
 *
 * @param[in] command comand from the command line;
 * @return(0 ) <0> if successful;
 * @return \f\$x if an error occurred;
 */
info::info(char *command) {
  // download ftp : //[<user>:<password>@]<host>/<url-path>\n";

  char needle[] = "ftp://";
  char *str_pointer = strstr(command, needle);

  str_pointer += strlen(needle);
  // cout << "str_pointer:" << str_pointer << endl;

  credential_manager(str_pointer);
  get_host_ip();
}

int info::validate() {
  if (validate_aux == false) {
    cout << "Invalid credentials " << endl;
    return -1;
  }
  if (!user) {
    cout << "NULL username" << endl;
    return -1;
  }
  if (!password) {
    cout << "NULL password" << endl;
    return -1;
  }
  if (!host_name) {
    cout << "NULL host name" << endl;
    return -1;
  }
  if (!host_ip) {
    cout << "NULL host ip" << endl;
    return -1;
  }
  if (!file_name) {
    cout << "NULL file name" << endl;
    return -1;
  }
  cout << "Valid credentials" << endl;
  return 0;
}

info::~info() {
  // if (user)
  //   delete user;
  // if (password)
  //   delete password;
  // if (host_name)
  //   delete host_name;
  // if (host_ip)
  //   delete host_ip;
  // if (file_name)
  //   delete file_name;
}

/**
 * @brief Auxiliary function to get the credentials;
 *
 * @param field user, password, etc.;
 * @param command
 * @param rejected
 */
void credentials_aux(char *field, char *command, const char *rejected, size_t *field_len) {
  *field_len = strcspn(command, rejected);
  memcpy(field, command, *field_len);
}

/**
 * @brief Save the user credentials or set user and password anonymous;
 *
 * @param command user input. If it's NULL user goes anonymous;
 * @return true for success and false otherwise.
 */
void info::credential_manager(char *command) {
  //   cout << "example: ./app download ftp://netlab1.fe.up.pt/pub.txt";
  size_t field_len;
//se nao houver user entramos em anonymous
  if (!command) {
    strcpy(user, "anonymous");
    strcpy(password, "anonymous");
    strcpy(user, "anonymous");
    strcpy(password, "anonymous");
  } else if (!strstr(command, "@")) {
    strcpy(user, "anonymous");
    strcpy(password, "anonymous");
    strcpy(user, "anonymous");
    strcpy(password, "anonymous");
  } else {
    credentials_aux(user, command, ":", &field_len);
    command += field_len + 1;
    credentials_aux(password, command, "@", &field_len);
    command += field_len + 1;
  }
  credentials_aux(host_name, command, "/", &field_len);
  command += field_len + 1;
  credentials_aux(file_name, command, "\0", &field_len);
  command += field_len + 1;

  // print();
}
/**
 * @brief Get the host ip from user host name.
 *
 * @return int
 */
int info::get_host_ip() {
  struct hostent *h;

  if ((h = gethostbyname(host_name)) == NULL) {
    cout << "Error gethostbyname \n";
    validate_aux = false;
    return -1;
  }
  strcpy(host_ip, inet_ntoa(*((struct in_addr *)h->h_addr)));

  return 0;
}

int validate_command(char *string) {
  if (!string)
    return EXIT_FAILURE;

  regex_t preg;
  const char *pattern = "^ftp://.*";
  // "^ftp://([a-z0-9]+:[a-z0-9]+@)?([\\.a-z0-9-]+)/([\\./a-z0-9-]+)";
  int rc;
  size_t nmatch = 2;
  regmatch_t pmatch[2];

  if (0 != (rc = regcomp(&preg, pattern, 0))) {
    printf("regcomp() failed, returning nonzero (%d)\n", rc);
    return EXIT_FAILURE;
  }

  if (0 != (rc = regexec(&preg, string, nmatch, pmatch, 0))) {
    printf("Failed to match '%s' with '%s',returning %d.\n",
           string, pattern, rc);
    cout << "Insert format: download ftp://[<user>:<password>@]<host>/<url-path>\n";
    cout << "example: ./app download ftp://netlab1.fe.up.pt/pub.txt\n";
    cout << "or\n";
    cout << "example: ./app ftp://rcom:rcom@netlab1.fe.up.pt/pipe.txt\n";
  }
  // printf("With the whole expression, "
  //        "a matched substring \"%.*s\" is found at position %d to %d.\n",
  //        pmatch[0].rm_eo - pmatch[0].rm_so, &string[pmatch[0].rm_so],
  //        pmatch[0].rm_so, pmatch[0].rm_eo - 1);
  // printf("With the sub-expression, "
  //        "a matched substring \"%.*s\" is found at position %d to %d.\n",
  //        pmatch[1].rm_eo - pmatch[1].rm_so, &string[pmatch[1].rm_so],
  //        pmatch[1].rm_so, pmatch[1].rm_eo - 1);

  regfree(&preg);
  return 0;
}
//User, password, host name and ip, file
void info::print() {
  if (user)
    printf("User: %s\n", user);
  if (password)
    printf("Password: %s\n", password);
  if (host_name)
    printf("Host name: %s\n", host_name);
  if (host_ip)
    printf("Host ip: %s\n", host_ip);
  if (file_name)
    printf("File path: %s\n", file_name);
}
