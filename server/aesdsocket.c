#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>

#include <signal.h>

#define PORT 9000
#define MAX_CONNECTION 5
#define VAR_TMP_AESDSOCKETDATA "/var/tmp/aesdsocketdata"
#define BUFFER_SIZE 8
#define NUMBER_OF_LINES 100000
#define ENDL '\n'

int caught_sigint = 0;
int caught_sigterm = 0;

void signal_handler(int signal_number) {
  if (signal_number == SIGINT) {
    syslog(LOG_DEBUG, "Caught SIGINT\n");
    caught_sigint = 1;
  } else if (signal_number == SIGTERM) {
    syslog(LOG_DEBUG, "Caught SIGTERM\n");
    caught_sigterm = 1;
  } else {
    // signal_number not handled
  }
  return;
}

int main(int argc, char* argv[]) {

  int is_daemon = 0;
  int opt;
  while ((opt = getopt(argc, argv, "-d")) != -1) {
    switch (opt) {
    case 'd':
      is_daemon = 1;
      break;
    default:
      break;
    }
  }

  syslog(LOG_DEBUG, "Starting as daemon: %s\n", (is_daemon) ? "true": "false");

  struct sigaction new_action = { 0 };
  new_action.sa_handler = signal_handler;

  int sigterm_status = sigaction(SIGTERM, &new_action, NULL);
  if (sigterm_status != 0) {
    syslog(LOG_ERR, "sigaction(SIGTERM, ...)");
    return -1;
  }

  int sigint_status = sigaction(SIGINT, &new_action, NULL);
  if (sigint_status != 0) {
    syslog(LOG_ERR, "sigaction(SIGINT, ...)");
    return -1;
  }

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    syslog(LOG_ERR, "socket()");
    return -1;
  }

  int option = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof server_address);
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);

  int bind_status = bind(server_fd, (struct sockaddr*)&server_address, sizeof(struct sockaddr_in));
  if (bind_status < 0) {
    syslog(LOG_ERR, "bind()");
    return -1;
  }


  if (is_daemon) {
    pid_t pid = fork();

    if (pid < 0) {
        syslog(LOG_ERR, "fork()\n");
        return -1;
    }
    else if (pid > 0) {
        syslog(LOG_DEBUG, "fork() successful - exiting - pid of child process: %d \n", pid);
        exit(EXIT_SUCCESS);
    }
    pid_t sid = setsid();
    if(sid < 0)
    {
      syslog(LOG_ERR, "bind()");
      return -1;
    }
  }


  int listen_status = listen(server_fd, MAX_CONNECTION);
  if (listen_status < 0) {
    syslog(LOG_ERR, "listen()");
    return -1;
  }

  int var_tmp_file_fd = open(VAR_TMP_AESDSOCKETDATA, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (var_tmp_file_fd < 0) {
    syslog(LOG_ERR, "open()");
    return -1;
  }

  struct sockaddr_in client_address = { 0 };
  socklen_t address_length = { 0 };

  while (1) {
    syslog(LOG_DEBUG, "Waiting for connection on port %d\n", PORT);
    int client_fd = accept(server_fd, (struct sockaddr*)&client_address, &address_length);
    // Breaking here because most of the time, we will be waiting for a connection.
    // If signal is caught, it will interrupt the system call.
    // And thus we can break the loop...
    if (caught_sigint || caught_sigterm) {
      syslog(LOG_DEBUG, "Caught signal, exiting\n");
      break;
    }
 
    if (client_fd < 0) {
      syslog(LOG_ERR, "accept()");
      syslog(LOG_DEBUG, "%s\n", strerror(errno));
      return -1;
    }

    char ip_adress[BUFFER_SIZE];
    inet_ntop(AF_INET, &(((struct sockaddr_in*)&client_address) -> sin_addr), ip_adress, INET_ADDRSTRLEN);
    syslog(LOG_DEBUG, "Accepted connection from %s\n", ip_adress);

    int in_size = 0;
    int available_size = BUFFER_SIZE;
    char* tmp_buffer;
    tmp_buffer = (char *)malloc(BUFFER_SIZE);
    if (!tmp_buffer) {
      syslog(LOG_ERR, "malloc(tmp_buffer, ...)");
      return -1;
    }
    long int n_line = 0;

    int interrupt = 0;

    while (!interrupt){
      int read_status = recv(client_fd, tmp_buffer, BUFFER_SIZE, 0);
      syslog(LOG_DEBUG, "Read %d bytes from socket %d\n", read_status, client_fd);
      if (read_status == 0 || (strchr(tmp_buffer, ENDL) != NULL)) { interrupt = 1; }
      if (read_status <= 0) continue;
      int write_status = write(var_tmp_file_fd, tmp_buffer, read_status);
      syslog(LOG_DEBUG, "Wrote %d bytes to %s using file descriptor %d\n", read_status, VAR_TMP_AESDSOCKETDATA, var_tmp_file_fd);
    }

    int total_to_send = lseek(var_tmp_file_fd, 0, SEEK_CUR);
    int total_sent = 0;
    while(total_sent < total_to_send) {
      lseek(var_tmp_file_fd, total_sent, SEEK_SET);
      int read_status = read(var_tmp_file_fd, tmp_buffer, BUFFER_SIZE);
      write(client_fd, tmp_buffer, read_status);
      total_sent += read_status;
    }
    lseek(var_tmp_file_fd, total_to_send, SEEK_SET);

    syslog(LOG_DEBUG, "Closed connection from %s\n", ip_adress);
    close(client_fd);

    if (tmp_buffer) {
      syslog(LOG_DEBUG, "Freeing memory from tmp_buffer \n");
      free(tmp_buffer); tmp_buffer = NULL;
    }

  }

  syslog(LOG_DEBUG, "Closing file %s\n", VAR_TMP_AESDSOCKETDATA);
  close(var_tmp_file_fd);
  syslog(LOG_DEBUG, "Closing server socket\n");
  close(server_fd);
  syslog(LOG_DEBUG, "Deleting file %s\n", VAR_TMP_AESDSOCKETDATA);
  int remove_status = remove(VAR_TMP_AESDSOCKETDATA);
  if (remove_status < 0) {
    syslog(LOG_ERR, "remove()");
    return -1;
  }
  
}