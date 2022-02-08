#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int open_file(){
    FILE *file = fopen("file_for_task3", "r");
    if (!file) {
        perror("File has not been opened");
    } else fclose(file);
    return 0;
}

int print_id(){
    printf("real ID : %d\n", getuid());
    printf("effective ID: %d\n", geteuid());
    }

int main() {
    print_id();
    open_file();
    setuid(getuid());
    print_id();
    open_file();
    return 0;
}