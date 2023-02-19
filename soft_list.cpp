#include "soft_list.h"
#include <stdlib.h>
#include "log.h"

void list_init(struct list* list)
{
    list->size = 10;
    list->head = (struct node*)calloc(list->size, sizeof(struct node));

    list->head[0].data = POISON;
    list->head[0].next = 0;
    list->head[0].prev = 0;

    for(int i = 1; i < list->size; i++)
    {
        list->head[i].data = POISON;
        list->head[i].next = -1;
        list->head[i].prev = -1;
    }
}

void list_delete(struct list* list)
{
    list->size = 0;
    free(list->head);
}

void list_print(FILE* log_file, struct list* list)
{
    for(int i = 0; i < list->size; i++)
    {
       // printf("%d / %d\t--->\t", list->head[i].data, i);
       fprintf(log_file, "Node number: %d\nData: %d\nPrev:%d\nNext:%d\n\n\t|\n\t|\n\t|\n\t\\/\n",
               i, list->head[i].data, list->head[i].prev, list->head[i].next);
    }
    fprintf(log_file, "--------------------\n");
}

void list_push(struct list* list, int num, data_t data)
{
    /*struct node node_prev = list->head[num];
    struct node node_next = list->head[list->head[num].next];*/

    int prev_num = list->head[num].next;
    int next_num = list->head[list->head[num].next].prev;

    /*int new_node_num = find_free_num(list);
    struct node node_new = list->head[new_node_num];

    node_prev.next = new_node_num;
    node_next.prev = new_node_num;

    node_new.data = data;
    node_new.prev = prev_num;
    node_new.next = next_num;*/

    struct node node_new = {};
    node_new.prev = prev_num;
    node_new.next = next_num;
    node_new.data = data;
    int new_node_code = find_free_num(list);

    if(new_node_code == -1)
    {
        fprintf(get_log_file(), "%d isn't pushed. List is overflow.", data);
        list_print(get_log_file(), list);
    }

    list->head[new_node_code] = node_new;

    list->head[num].next = new_node_code;
    list->head[list->head[num].next].prev = new_node_code;
    list_print(get_log_file(), list);
}

int find_free_num(struct list* list)
{
    for(int i = 0; i < list->size; i++)
        if(list->head[i].next == -1)
            return i;

    return -1;
}
