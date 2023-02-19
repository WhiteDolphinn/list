#ifndef SOFT_LIST_H
#define SOFT_LIST_H

#include <stdio.h>
#define POISON (int)0xDED32DED

typedef int data_t;

struct node{
    int prev;
    data_t data;
    int next;
};

struct list{
    struct node* head;
    int size;
};

void list_init(struct list* list);
void list_delete(struct list* list);
void list_print(FILE* log_file, struct list* list);

#endif
