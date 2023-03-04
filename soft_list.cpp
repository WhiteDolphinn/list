#include "soft_list.h"
#include <stdlib.h>
#include "queue/log.h"
#include "dotter.h"

static int find_free_num(struct list* list);
static void node_free(struct list* list, int node_num);

void list_init(struct list* list)
{
    list->size = 10;
    list->head = (struct node*)calloc(list->size, sizeof(struct node));
    list->free_nodes = (struct queue*)calloc(1, sizeof(struct queue));

    queue_init(list->free_nodes);

    list->head[0].data = POISON;
    list->head[0].next = 0;
    list->head[0].prev = 0;

    for(int i = 1; i < list->size; i++)
    {
        //list->head[i].next = (i + 1) % (list->size);
        list->head[i].next = -1;
        list->head[i].prev = -1;
        list->head[i].data = POISON;
        queue_push(list->free_nodes, i);
    }
}

void list_delete(struct list* list)
{
    list->size = 0;
    free(list->head);
    queue_delete(list->free_nodes);
    free(list->free_nodes);
}

void list_print_old(FILE* log_file, struct list* list)
{
    for(int i = 0; i < list->size; i++)
    {
       // printf("%d / %d\t--->\t", list->head[i].data, i);
       fprintf(log_file, "Node number: %d\nData: %d\nPrev:%d\nNext:%d\n\n\t||\n\t||\n\t||\n\t\\/\n",
               i, list->head[i].data, list->head[i].prev, list->head[i].next);
    }
    fprintf(log_file, "-------------------------------------------------------------\n");
}

void list_print(struct list* list)
{
    struct node* current_node = list->head;
    graph_start();

    void* current_node_address = (void*)current_node;
    void* next_node_address = (void*)(&(list->head[current_node->next]));
    void* prev_node_address = (void*)(&(list->head[current_node->prev]));

    do
    {
        current_node_address = (void*)current_node;
        next_node_address = (void*)(&(list->head[current_node->next]));
        prev_node_address = (void*)(&(list->head[current_node->prev]));

        graph_add_dot(current_node_address, current_node->data,
        next_node_address, prev_node_address);

        current_node = &(list->head[current_node->next]);

    }while(current_node != list->head);

    do
    {
        current_node_address = (void*)current_node;
        next_node_address = (void*)(&(list->head[current_node->next]));
        prev_node_address = (void*)(&(list->head[current_node->prev]));

        graph_add_arrow(current_node_address, next_node_address, "#D0D0FF");
        graph_add_arrow(current_node_address, prev_node_address, "#FFD0D0");

        current_node = &(list->head[current_node->next]);

    }while(current_node != list->head);

    graph_end();
}

void list_push(struct list* list, int num, data_t data)
{
    int prev_num = num;
    int next_num = list->head[num].next;

    struct node node_new = {};
    node_new.prev = prev_num;
    node_new.next = next_num;
    node_new.data = data;
    //int new_node_code = find_free_num(list);
    int new_node_code = queue_pop(list->free_nodes);

    if(new_node_code == POISON)
    {
        fprintf(get_log_file(".txt"), "%d isn't pushed. List is overflow.", data);
        //list_print(get_log_file(".txt"), list);
        list_print_old(get_log_file(".txt"), list);
        return;
    }

    list->head[new_node_code] = node_new;

    list->head[num].next = new_node_code;

    list->head[next_num].prev = new_node_code;

    list_print_old(get_log_file(".txt"), list);
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

    if(prev_node_num == -1 || next_node_num == -1)
    {
        fprintf(get_log_file(".txt"), "%d is uncorrect num of node.\n", node_num);
        list_print_old(get_log_file(".txt"), list);
        return POISON;
    }

    list->head[prev_node_num].next = next_node_num;
    list->head[next_node_num].prev = prev_node_num;

    node_free(list, node_num);

    list_print_old(get_log_file(".txt"), list);
    return data;
}

static void node_free(struct list* list, int node_num)
{
    list->head[node_num].data = POISON;
    list->head[node_num].prev = -1;

    list->head[node_num].next = -1;
    queue_push(list->free_nodes, node_num);
    //list->head[node_num].next = list->head[list->head[node_num].next].next;
}
