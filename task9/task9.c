#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char *filename_child = malloc(100 * sizeof(char));
    char *filename_parent = malloc(100 * sizeof(char));
    printf("Input text file filename_child\n");
    scanf("%s", filename_child);
    printf("Input text file filename_parent\n");
    scanf("%s", filename_parent);
    char *command = "cat ";
     pid_t child_process = fork();
    if (child_process == -1) {
        perror("Creating child process error\n");
        return -1;
    } else if (child_process == 0) {
        execlp("cat", command, filename_child, NULL);
        perror("Executing command in child process error\n");
        return -1;
    } else if (child_process > 0) {
        while (wait(0) != -1);
        int fd = open(filename_parent, O_RDONLY);
        off_t pos = lseek(fd, 0, SEEK_END);
        size_t size;
        if (pos == (off_t) - 1) {
            size = -1;
        }
        size = (size_t) pos + 1;
        lseek(fd, 0, SEEK_SET);
        char *buf = malloc(size * sizeof(char));
        if (!buf) {
            perror("Memory allocation error\n");
            return -1;
        }
        read(fd, buf, size);
        write(STDIN_FILENO, buf, size);
        if (close(fd) == -1) {
            perror("close() error\n");
            return -1;
        }
    }
return 0;

}