#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>

int write_process(int pipe_fd[2]) {
    if (close(pipe_fd[0]) == -1) {
        perror("close() error");
        return 1;
    }
    const char *message = "Hello, another process!";
    int i = 0;
    if (write(pipe_fd[1], message, strlen(message)) == -1) {
        perror("write() error");
        if (close(pipe_fd[1]) == -1) {
            perror("close() error");
        }
        return 1;
    }
    printf("Start text: %s\n", message);
    if (close(pipe_fd[1]) == -1) {
        perror("close() error");
        return 1;
    }
    return 0;
}

int read_process(int pipe_fd[2]) {
    if (close(pipe_fd[1]) == -1) {
        perror("close() error");
        return 1;
    }
    char buf[256];
    size_t read_count = read(pipe_fd[0], buf, 256);
    if (read_count == -1 && errno != EINTR) {
        perror("read() error");
        if (close(pipe_fd[0]) == -1) {
            perror("close() error");
        }
        return 1;
    }
    size_t i;
    for (i = 0; i < read_count; i++) {
        buf[i] = (char) toupper(buf[i]);
    }
    char read_string[read_count + 1];
    strncpy(read_string, buf, read_count);
    read_string[read_count] = '\0';
    printf("Upper formatted text: %s\n", read_string);
    if (close(pipe_fd[0]) == -1) {
        perror("close() error");
        return 1;
    }
    return 0;
}

int close_pipe(int pipe_fd[2]) {
    if (close(pipe_fd[0]) == -1) {
        perror("close() error");
        return 1;
    }
    if (close(pipe_fd[1]) == -1) {
        perror("close() error");
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int pipe_fd[2];
    int status = 0;
    if (pipe(pipe_fd) == -1) {
        perror("pipe() error");
        return 0;
    }
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork() error");
        close_pipe(pipe_fd);
        return 1;
    } else if (pid == 0) {
        if (read_process(pipe_fd) == 1) {
          perror("read error");
            return 1;
        }
    } else if (pid > 0) {
        if (write_process(pipe_fd) == 1) {
            perror("write error");
            return 1;
        }
        while (WIFEXITED(status) || WIFSIGNALED(status)) {
            pid_t child_pid = waitpid(pid, &status, 0);
            if (child_pid == -1) {
                perror("waitpid() error: ");
                return 1;
            }
            if (WIFEXITED(status)) {
                printf("exited, status=%d\nchild_pid=%d\n", WEXITSTATUS(status), (int) pid);
                return 0;
            } else if (WIFSIGNALED(status)) {
                printf("killed by signal %d\nchild_pid=%d\n", WTERMSIG(status), (int) pid);
                return 0;
            } else if (WIFSTOPPED(status)) {
                printf("stopped by signal %d\nchild_pid=%d\n", WSTOPSIG(status), (int) pid);
                return 0;
            } else if (WIFCONTINUED(status)) {
                printf("continued\n");
                return 0;
            }
        }
    }
    return 0;
}