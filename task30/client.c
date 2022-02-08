#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define BUF_SIZE 256

int main() {
    struct sockaddr_un address;
    int bytes_written = 0;

    char *msg = "hello world!\n";
    char *path = "file";

    int s = socket(AF_UNIX, SOCK_STREAM, 0);
    if(s < 0) {
        perror("s() error");
        return -1;
    }

    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, path);

    int addrlen = sizeof(address);
    if(connect(s, (struct sockaddr*)&address, (socklen_t)addrlen) < 0) {
        perror("connect() error");
        return -1;
    }

    send(s, msg, strlen(msg), 0);

    if(close(s) < 0){
        perror("close() error");
        return -1;
    }
    return 0;
}