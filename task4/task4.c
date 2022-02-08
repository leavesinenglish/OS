#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 255

typedef struct Node {
    char *string;
    struct Node *next;
} Node;

typedef struct List {
    struct Node *first;
} List;

List *create_list() {
    List *List_ = (List *) malloc(sizeof(List));
    if (List_)
        List_->first = NULL;
    return List_;
}

Node *new_elem() {
    Node *elem = (Node *) malloc(sizeof(Node));
    if (elem) {
        elem->next = NULL;
    }
    return elem;
}

void free_list(List *list) {
    if (list) {
        Node *current = list->first;
        while (current) {
            if (current->string)
                free(current->string);
            Node *temp = current;
            current = current->next;
            free(temp);
        }
    }
}

void insert_elem(List *list, char *str) {
    if (list == NULL || str == NULL)
        return;
    if (list->first == NULL) {
        Node *elem = new_elem();
        if (elem) {
            list->first = elem;
            elem->string = malloc(sizeof(char) * (strlen(str) + 1)); 
                        if (current->next->string) {
                strcpy(current->next->string, str);
                current->next->next = NULL;
            } else free_list(list);
        } else return;
    }
}


void print_list(List *list) {
    if (list) {
        Node *current = list->first;
        while (current) {
            printf("%s", current->string);
            current = current->next;
        }
    }
}

char *get_string(char *buf) {
    if (buf == NULL)
        return NULL;
    char *string = fgets(buf, MAX_SIZE, stdin);
    return string;
    
int main(void) {
    List *List_ = create_list();
    if (List_ == NULL) {
        perror("Malloc: couldn't allocate memory");
        return -1;
    }
    char buf[MAX_SIZE];
    while (1) {
        char *string = get_string((char *) &buf);
        if (string[0] != '.') {
            insert_elem(List_, string);
        } else break;
    }
    print_list(List_);
    free_list(List_);
    return 0;
}