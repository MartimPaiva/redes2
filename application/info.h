#pragma once

#include "stdio.h"
#include <regex.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <iostream>
using namespace std;

class info {
private:
  int get_host_ip();
  void credential_manager(char *command);
  bool validate_aux = true;

public:
  info(char *url);
  ~info();
  void print();
  int validate();

  char user[512] = {};
  char password[512] = {};
  char host_name[512] = {};
  char host_ip[512] = "Not def yet";
  char file_name[512] = {};
};

int validate_command(char *arg);