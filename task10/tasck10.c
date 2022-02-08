#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int fork_prog(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Incorrect number of arguments\n");
        return 1;
    }
    pid_t child = fork(); //creating child process
    switch (child) {
        case -1: {
            perror("Error in fork");
            return 1;
        }
        case 0: {
            execv(argv[1], argv);
            perror("Error in execv");
            return 1;
        }
        default: {
            int status;
            pid_t ChildPid;
            while (WIFEXITED(status) || WIFSIGNALED(status)) {
                ChildPid = waitpid(child, &status, 0); // wait for child process to change state
                if (ChildPid == -1) {
                    perror("Error in waitpid");
                    return 1;
                }
                if (WIFEXITED(status)) { //returns a nonzero value if the child process terminated normally with exit or _exit.
                    printf("Exit code =  %d\n", WEXITSTATUS(status));
                    return 0;
                }
                else if (WIFSIGNALED(status)){
                    printf("Signal code = %d\n", WTERMSIG(status));
                    return 0;
                }
            }
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    int return_val = fork_prog(argc, argv);
    return return_val;
}