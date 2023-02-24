#include "soft_list.h"
#include <stdlib.h>
#include "log.h"

static int find_free_num(struct list* list);
static void node_free(struct node* node);

void list_init(struct list* list)
{
    list->size = 10;
    list->head = (struct node*)calloc(list->size, sizeof(struct node));

    list->head[0].data = POISON;
    list->head[0].next = 0;
    list->head[0].prev = 0;

    for(int i = 1; i < list->size; i++)
    {
        /*list->head[i].data = POISON;
        list->head[i].next = -1;
        list->head[i].prev = -1;*/
        node_free(&(list->head[i]));
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
    fprintf(log_file, "-------------------------------------------------------------\n");
}

void list_push(struct list* list, int num, data_t data)
{
    int prev_num = num;
    int next_num = list->head[num].next;

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

    list->head[next_num].prev = new_node_code;

    list_print(get_log_file(), list);
}

static int find_free_num(struct list* list)
{
    for(int i = 0; i < list->size; i++)
        if(list->head[i].next == -1)
            return i;

    return -1;
}

data_t list_pop(struct list* list, int node_num)
{
    data_t data = list->head[node_num].data;

    int prev_node_num = list->head[node_num].prev;
    int next_node_num = list->head[node_num].next;

    list->head[prev_node_num].next = next_node_num;
    list->head[next_node_num].prev = prev_node_num;

    /*list->head[node_num].data = POISON;
    list->head[node_num].prev = -1;
    list->head[node_num].next = -1;*/
    node_free(&(list->head[node_num]));

    list_print(get_log_file(), list);
    return data;
}

static void node_free(struct node* node)
{
    node->data = POISON;
    node->next = -1;
    node->prev = -1;
}
