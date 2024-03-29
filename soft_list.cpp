#include "soft_list.h"
#include <stdlib.h>
#include "queue/log.h"
#include "dotter.h"

//#define BLOCK_LIST_CHECK

static void node_free(struct list* list, int node_num);
static void list_check(struct list* list);
static void linearization(struct list* list, unsigned int num_of_nodes);

void list_init(struct list* list, unsigned int list_size)
{
    list->size = list_size;
    list->head = (struct node*)calloc(list->size, sizeof(struct node));
    list->free_nodes = (struct queue*)calloc(1, sizeof(struct queue));
    list->is_correct = true;
    list->is_deleted = false;

    if(list->head == nullptr || list->free_nodes == nullptr)
    {
        fprintf(get_log_file(".txt"), "Error with opening head or free_nodes\n");
        list->is_correct = false;
        return;
    }

    queue_init(list->free_nodes, list->size);

    list->head[0].data = POISON;
    list->head[0].next = 0;
    list->head[0].prev = 0;

    for(int i = 1; i < (int)list->size; i++)
        node_free(list, i);

    list_check(list);
}

void list_delete(struct list* list)
{
    if(list->is_deleted == false)
    {
        list->size = 0;
        free(list->head);
        queue_delete(list->free_nodes);
        free(list->free_nodes);
        list->head = nullptr;
        list->free_nodes = nullptr;
        list->is_deleted = true;
    }
}

void list_print_old(FILE* log_file, struct list* list)
{
    if(list->is_deleted == false)
    {
        for(int i = 0; i < (int)list->size; i++)
        fprintf(log_file, "Node number: %d\nData: %d\nPrev:%d\nNext:%d\n\n\t||\n\t||\n\t||\n\t\\/\n",
                i, list->head[i].data, list->head[i].prev, list->head[i].next);

        fprintf(log_file, "-------------------------------------------------------------\n");
    }
}

void list_print(struct list* list)
{
    if(list->is_deleted)    return;

    graph_start();

    char* num_of_arrows = (char*)calloc(list->size, sizeof(char));   //если не КАЛЛочить, то варнинг

    unsigned int current_node_index = 0;

    for( ; current_node_index < list->size; current_node_index++)
    {
        void* next_node_address = (void*)(&(list->head[list->head[current_node_index].next]));
        void* prev_node_address = (void*)(&(list->head[list->head[current_node_index].prev]));

        graph_add_arrow((void*)(&(list->head[current_node_index])), next_node_address, "#D0D0FF");
        graph_add_arrow((void*)(&(list->head[current_node_index])), prev_node_address, "#FFD0D0");

        if(list->head[current_node_index].next != -1)
            num_of_arrows[list->head[current_node_index].next]++;

        if(list->head[current_node_index].next != -1)
            num_of_arrows[list->head[current_node_index].prev]++;
    }

    current_node_index = 0;

    graph_add_dot((void*)(&(list->head[-1])), POISON, nullptr, nullptr, "#0000FF");

    for( ; current_node_index < list->size; current_node_index++)
    {
        void* next_node_address = (void*)(&(list->head[list->head[current_node_index].next]));
        void* prev_node_address = (void*)(&(list->head[list->head[current_node_index].prev]));

        switch(num_of_arrows[current_node_index])
        {
            case 2:
            {
                graph_add_dot((void*)(&(list->head[current_node_index])), list->head[current_node_index].data,
                next_node_address, prev_node_address, "#FFD0DC");
                break;
            }
            case 0:
            {
                graph_add_dot((void*)(&(list->head[current_node_index])), list->head[current_node_index].data,
                next_node_address, prev_node_address, "#00FF00");
                break;
            }
            default:
            {
                graph_add_dot((void*)(&(list->head[current_node_index])), list->head[current_node_index].data,
                next_node_address, prev_node_address, "#FF0000");
            }
        }
    }

    graph_add_head(list->head);
    graph_end();
    free(num_of_arrows);
}

void list_push(struct list* list, int num, data_t data)
{
    list_check(list);
    if(list->is_correct == true)
    {
        int prev_num = num;
        int next_num = list->head[num].next;

        struct node node_new = {};
        node_new.prev = prev_num;
        node_new.next = next_num;
        node_new.data = data;

        int new_node_code = queue_pop(list->free_nodes);

        if(new_node_code == POISON)
        {
            list_resize(list, (int)list->size);
            list_print_old(get_log_file(".txt"), list);

            new_node_code = queue_pop(list->free_nodes);

            if(new_node_code == POISON)
            {
                fprintf(get_log_file(".txt"), "Error in resize queue\n");
                return;
            }
        }

        list->head[new_node_code] = node_new;

        list->head[num].next = new_node_code;

        list->head[next_num].prev = new_node_code;

        list_print_old(get_log_file(".txt"), list);
        list_check(list);
    }
}

void list_push_head(struct list* list, data_t data)
{
    list_push(list, 0, data);
}

void list_push_tail(struct list* list, data_t data)
{
    list_push(list, list->head[0].prev, data);
}

int list_index_by_data(struct list* list, data_t data)
{
    struct node* current_node = list->head;

    do
    {
        if(data == current_node->data)
            return list->head[current_node->prev].next;

        current_node = &(list->head[current_node->next]);

    }while(current_node != list->head);

    return POISON;
}

int list_node_num_by_data(struct list* list, data_t data)
{
    if(list->head[0].data == data)
        return 0;

    struct node* current_node = &(list->head[list->head[0].next]);

    for(int i = 1; current_node != list->head; i++)
    {
        if(data == current_node->data)
            return i;

        current_node = &(list->head[current_node->next]);
    }

    return POISON;
}

data_t list_pop(struct list* list, int node_num)
{
    list_check(list);

    if(list->is_correct == true)
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
        list_check(list);

        return data;
    }
    return POISON;
}

data_t list_data_by_index(struct list* list, unsigned int index)
{
    return list->head[index].data;
}

data_t list_data_by_node_num(struct list* list, unsigned int node_num)
{
    struct node* current_node = list->head;
    for(unsigned int i = 0; i < node_num; i++)
    {
        current_node = &(list->head[current_node->next]);
    }
    return current_node->data;
}

bool list_resize(struct list* list, int extra_mem)
{
    int old_size = (int)list->size;
    int new_size = (int)list->size + extra_mem;

    if(new_size < 0)
    {
        fprintf(get_log_file(".txt"), "Error with list resize. new_size < 0\n");
        return false;
    }

    struct node* buff_ptr = (struct node*)realloc(list->head, (unsigned)new_size * sizeof(struct node));

    if(buff_ptr == nullptr)
        return false;

    list->head = buff_ptr;

    list->size = (unsigned int)new_size;

    if(!queue_resize(list->free_nodes, list->size))
        return false;

    for(int i = old_size; i < (int)list->size; i++)
        node_free(list, i);

    return true;
}

static void node_free(struct list* list, int node_num)
{
    list->head[node_num].data = POISON;
    list->head[node_num].prev = -1;

    list->head[node_num].next = -1;
    queue_push(list->free_nodes, node_num);
}

#ifndef BLOCK_LIST_CHECK

static void list_check(struct list* list)
{
    if(list->is_deleted == false)
    {
        if(list->head[0].data != POISON)
        {
            fprintf(get_log_file(".txt"), "Error with data in head\n");
            list->is_correct = false;
        }

        unsigned int num_of_nodes = 0;
        node* current_node = list->head;
        do
        {
            if(current_node->next == -1 || current_node->prev == -1)
            {
                fprintf(get_log_file(".txt") ,"Error with arrow in %p node", (void*)current_node);
                list->is_correct = false;
            }

            current_node = &(list->head[current_node->next]);
            num_of_nodes++;
        }while(current_node != list->head);

        if(num_of_nodes * 3 < list->size && list->size > 5)
            linearization(list, num_of_nodes);

        if(list->is_correct == false)
        {
            list_print(list);
            list_delete(list);
        }
    }
}
#else

static void list_check(struct list* list)
{
    return;
}

#endif

static void linearization(struct list* list, unsigned int num_of_nodes)
{
    data_t* buf_data = (data_t*)calloc(num_of_nodes, sizeof(data_t));

    node* current_node = list->head;
    for(unsigned int i = 0; i < num_of_nodes; i++)
    {
        list_print_old(get_log_file(".txt"), list);
        buf_data[i] = current_node->data;
        current_node = &(list->head[current_node->next]);
        fprintf(get_log_file(".txt"), "buf_data[%u] = %d\n", i, buf_data[i]);
    }


    for(unsigned int i = 0; i < num_of_nodes; i++)
    {
        list->head[i].data = buf_data[i];
        list->head[i].prev = (int)i - 1;
        list->head[i].next = (int)i + 1;
    }
    list->head[0].prev = (int)num_of_nodes - 1;
    list->head[num_of_nodes-1].next = 0;

    queue_clean(list->free_nodes);
    fprintf(get_log_file(".txt"), "queue_clean have worked\n num_of_nodes = %u", num_of_nodes);
    for(unsigned int i = num_of_nodes; i < (list->size/2); i++)
    {
        node_free(list, (int)i);
        //queue_push(list->free_nodes, (int)i);
    }

    list_resize(list, -(int)(list->size/2));
    free(buf_data);
}
