#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#define MAX_SIZE 256

int empty_string_counter() {
    FILE *pipe;
    if ((pipe = (FILE *) popen("grep ^$ file.txt | wc -l ", "r")) == NULL) {
        perror("popen() error \n");
        return 1;
    }
    char buffer[MAX_SIZE];
    char *amountOfEmpty;
    char *endPtr;
    if ((amountOfEmpty = fgets(buffer, MAX_SIZE, pipe)) == NULL && !feof(pipe)) {
        perror("fgets() error \n");
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
    printf("Amount of empty strings is %ld\n", strtol(amountOfEmpty, &endPtr, 10));
    return 0;
}

int main() {
    return empty_string_counter();
}