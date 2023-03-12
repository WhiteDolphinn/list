#ifndef SOFT_LIST_H
#define SOFT_LIST_H

#include <stdio.h>
#include "queue/queue_soft.h"
//#define POISON (int)0xDED32DED

typedef int data_t;

struct node{
    int prev;
    data_t data;
    int next;
};

struct list{
    struct node* head;
    unsigned int size;
    struct queue* free_nodes;
    bool is_correct;
    bool is_deleted;
};

void list_init(struct list* list);
void list_delete(struct list* list);
void list_print_old(FILE* log_file, struct list* list);
void list_print(struct list* list);
void list_push(struct list* list, int num, data_t data);
void list_resize(struct list* list);
int search_node(struct list* list, data_t data);
data_t list_pop(struct list* list, int node_num);

#endif
