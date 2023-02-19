#ifndef SOFT_LIST_H
#define SOFT_LIST_H

typedef int data_t;

struct node{
    struct node* prev;
    data_t data;
    struct node* next;
};

struct list{
    struct node* head;
    int size;
};

void list_init(struct list* list);
void list_delete(struct list* list);

#endif
