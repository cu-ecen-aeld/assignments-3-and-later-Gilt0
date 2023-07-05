#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <syslog.h>

#define BUFFER 4096


int main(int argc, char* argv[]) {

    openlog(NULL, 0, LOG_USER);

    char info_message[3*BUFFER];
    char error_message[2*BUFFER];

    if (argc < 3) {
        sprintf(error_message, "Invalid number of arguments (received %d - expected 2)", argc - 1);
        syslog(LOG_ERR, "%s", error_message);
        closelog();
        printf("Error: %s\n", error_message);
        printf("%s\n", "");
        printf("%s\n", "Usage");
        printf("%s\n", "./writer.sh <writefile> <writestr>");
        printf("%s\n", "<writefile> is a file name");
        printf("%s\n", "<writestr>  is string to write to <writefile>");
        return 1;
    }

    char filename[BUFFER];
    char text_to_write[BUFFER];
    strcpy(filename, argv[1]);
    strcpy(text_to_write, argv[2]);

    int fd;
    fd = open(filename, O_WRONLY | O_TRUNC);
    if (fd == -1) {
        sprintf(error_message, "errno %d - %s - filename provided: %s", errno, strerror(errno), filename);
        syslog(LOG_ERR, "%s", error_message);
        closelog();
        printf("Error: %s\n", error_message);
        printf("%s\n", "");
        printf("%s\n", "Usage");
        printf("%s\n", "./writer.sh <writefile> <writestr>");
        printf("%s\n", "<writefile> is a file name");
        printf("%s\n", "<writestr>  is string to write to <writefile>");
        return 1;
    }

    size_t size = strlen (text_to_write);
    ssize_t status = write (fd, text_to_write, size);
    if (status == -1) {
        close(fd);
        sprintf(error_message, "errno %d - %s\n", errno, strerror(errno));
        syslog(LOG_ERR, "%s", error_message);
        closelog();
        printf("Error: %s\n", error_message);
        printf("%s\n", "");
        printf("%s\n", "Usage");
        printf("%s\n", "./writer.sh <writefile> <writestr>");
        printf("%s\n", "<writefile> is a file name");
        printf("%s\n", "<writestr>  is string to write to <writefile>");
        return 1;
    }
    else if (size != status) {
        close(fd);
        sprintf(error_message, "errno nan - buffer not written entirely (%ld characters written - expected %ld characaters to be written)", size, status);
        syslog(LOG_ERR, "%s", error_message);
        closelog();
        printf("Error: %s\n", error_message);
        printf("%s\n", "");
        printf("%s\n", "Usage");
        printf("%s\n", "./writer.sh <writefile> <writestr>");
        printf("%s\n", "<writefile> is a file name");
        printf("%s\n", "<writestr>  is string to write to <writefile>");
        return 1;
    }
    sprintf(info_message, "Writing %s to %s”", text_to_write, filename);
    syslog(LOG_DEBUG, "%s", info_message);
    closelog();
    close(fd);
    return 0;

}