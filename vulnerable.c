#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 12345

int getPassword(int socket_fd, char * password_buf);
int accessGranted(int socket_fd);
int accessDenied(int socket_fd);

int main()
{
  char password[16];
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[100] = {0};

  char * hello = "Welcome to Super Sekrit Program to control teh missiles!\r\nPlease enter the super sekrit password: ";


  printf("Creating socket\n");

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
      perror("socket failed");
      exit(EXIT_FAILURE);
  }

  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                &opt, sizeof(opt)))
  {
      perror("setsockopt");
      exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( PORT );

  // Forcefully attaching socket to the port 8080
  if (bind(server_fd, (struct sockaddr *)&address,
                               sizeof(address))<0)
  {
      perror("bind failed");
      exit(EXIT_FAILURE);
  }
  if (listen(server_fd, 3) < 0)
  {
      perror("listen");
      exit(EXIT_FAILURE);
  }
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                     (socklen_t*)&addrlen))<0)
  {
      perror("accept");
      exit(EXIT_FAILURE);
  }


  // Send Hello Message
  send(new_socket, hello, strlen(hello), 0);

  // Receive Password
  getPassword(new_socket, password);

  send(new_socket, "\r\n", 2, 0);
  send(new_socket, "\r\n", 2, 0);

  // Send Unauthorized Message
  accessDenied(new_socket);

  close(new_socket);
  close(server_fd);

  return 0;

}

int getPassword(int socket_fd, char * password_buf)
{

  char password[16];
  read(socket_fd, password, 100);
  // strcpy(password_buf, password);

  int i = 0;
  while (password[i] != 0) {
    password_buf[i] = password[i];
    i++;
  }

  return 0;
}

int accessGranted(int socket_fd)
{

  char command[24] = {0};
  char * auth = "##############################  AUTHORIZED  ##############################\r\n";
  char * auth_msg = "####  Access has been granted, you have full control of the missles!  ####\r\n";
  char * commands = "COMMANDS:\r\n1) FIRE MISSILES AT UNITED STATES\r\n2) SELF DESTRUCT\r\n3) DISABLE ALL MISSILES\r\n";
  char * fire_missiles = "FIRING FIRING MISSILES AT UNITED STATES IN 3, 2, 1...\r\n";
  char * self_destruct = "SELF DESTRUCT ACTIVATED, THIS FACILITY WILL EXPLODE IN 10, 9, 8...\r\n";
  char * disable = "MISSILE LAUNCH SYSTEM HAS BEEN DEACTIVATED\r\n";
  char * no_command = "A VALID COMMAND WAS NOT ENTERED, EXITING SYSTEM\r\n";

  send(socket_fd, auth, strlen(auth), 0);
  send(socket_fd, auth_msg, strlen(auth_msg), 0);
  send(socket_fd, auth, strlen(auth), 0);
  send(socket_fd, commands, strlen(commands), 0);

  read(socket_fd, command, 23);

  if (strcmp(command, "1") == 0) {
    send(socket_fd, fire_missiles, strlen(fire_missiles), 0);
  } else if (strcmp(command, "2") == 0) {
    send(socket_fd, self_destruct, strlen(self_destruct), 0);
  } else if (strcmp(command, "3") == 0) {
    send(socket_fd, disable, strlen(disable), 0);
  } else {
    send(socket_fd, no_command, strlen(no_command), 0);
  }

  return 0;
}

int accessDenied(int socket_fd)
{

  char * unauth = "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  UNAUTHORIZED  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n";
  char * unauth_msg = "!!!!  Unauthorized access attempted, the authorities will be notified !!!!\r\n";

  send(socket_fd, unauth, strlen(unauth), 0);
  send(socket_fd, unauth_msg, strlen(unauth_msg), 0);
  send(socket_fd, unauth, strlen(unauth), 0);

  return 0;
}
