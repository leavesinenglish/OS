#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>

#define BUF_SIZE 256

int main() {
    int s = socket(AF_UNIX, SOCK_STREAM, 0);
    if (s < 0) {
        perror("s() error");
        return -1;
    }
    struct sockaddr_un address;
    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    char *path = "file";
    strcpy(address.sun_path, path);
    if (bind(s, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("bind() error");
        unlink(path);
        return -1;
    }
    int backlog = 1;
    if (listen(s, backlog) < 0) {
        perror("listen() error");
        unlink(path);
        return -1;
    }
    struct sockaddr_un client_address;
    socklen_t client_address_len = sizeof(client_address);
    int client_fd = accept(s,
                           (struct sockaddr *) &client_address, (socklen_t *) &client_address_len);
    if(client_fd < 0) {
        perror("accept() error");
        unlink(path);
        return -1;
    }
    int size = 0;
    char string[BUF_SIZE] = {};
    while((size = read(client_fd, string, BUF_SIZE)) == -1) {
        if (errno != EINTR) {
            perror("read() error");
            unlink(path);
            return -1;
        }
    }
    size_t i;
    for (i = 0; i < size; ++i)
        string[i] = toupper(string[i]);
    printf("%s\n", string);
    unlink(path);
    return 0;
}