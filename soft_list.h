#ifndef SOFT_LIST_H
#define SOFT_LIST_H

#include <stdio.h>
#include "queue/queue_soft.h"

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

void list_init(struct list* list, unsigned int list_size);
void list_delete(struct list* list);
void list_print_old(FILE* log_file, struct list* list);
void list_print(struct list* list);
void list_push(struct list* list, int num, data_t data);
void list_push_head(struct list* list, data_t data);
void list_push_tail(struct list* list, data_t data);
//void list_pop_head()
//void list_pop_tail()
bool list_resize(struct list* list, int extra_mem);
int list_index_by_data(struct list* list, data_t data); //найти номер нода в массиве по значению
int list_node_num_by_data(struct list* list, data_t data); //найти номер узла по значению
data_t list_data_by_index(struct list* list, unsigned int index); //значение по номеру нода в массиве
data_t list_data_by_node_num(struct list* list, unsigned int node_num); //значение по номеру узла


data_t list_pop(struct list* list, int node_num);

#endif
