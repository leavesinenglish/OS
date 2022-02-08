#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    FILE *pipe = popen("echo Hello, another process!", "r");
    if (pipe == NULL) {
        perror("popen() error");
        return 1;
    }
    char buf[256];
    size_t read_count = fread(buf, 1, 256, pipe);
    if (ferror(pipe) == -1) {
        perror("fread() error");
        int status = pclose(pipe);
        if (status == -1) {
            perror("pclose() error \n");
            return 1;
        } else {
            if (WIFEXITED(status)) {
                printf("Child terminated normally, status = %d\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("Killed by signal with number %d\n", WTERMSIG(status));
            } else if (WIFSTOPPED(status)) {
                printf("stopped by signal %d\n", WSTOPSIG(status));
            }
        }
        return 1;
    }
    int status = pclose(pipe);
    if (status == -1) {
        perror("pclose() error \n");
        return 1;
    } else {
        if (WIFEXITED(status)) {
            printf("Child terminated normally, status = %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Killed by signal with number %d\n", WTERMSIG(status));
        } else if (WIFSTOPPED(status)) {
            printf("stopped by signal %d\n", WSTOPSIG(status));
        }
    }
    size_t i;
    for (i = 0; i < read_count; i++) {
        buf[i] = (char) toupper(buf[i]);
    }
    size_t written_count = fwrite(buf, read_count, 1, stdout);
    if (ferror(pipe) == -1) {
        perror("fwrite() error");
        return 1;
    }
    return 0;
}