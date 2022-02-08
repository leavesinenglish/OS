#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
extern char **environ;

int execvpe(const char *file, char* const argv[], char* envp[]) {
    if (clearenv()) {
        perror("clearenv() error:");
        return -1;
    }
    int i;
    environ = envp;
    if (execvp(file, argv)) {
        perror("execvp(2) error:");
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    char *args[] = {"Argument 1", "Argument 2", "Argument 3", NULL};
    char *envp[] = {"e
nv1 = 1 ", "env2 = 2", "env3 = 3", NULL};
    char **prev = environ;
    if (execvpe(argv[1], args, envp) == -1) {
        perror("execvpe(3) error:");
        environ = prev;
        return -1;
    }
    return 0;
}