#include "soft_list.h"
#include "queue/log.h"

int main()
{
    struct list list = {};
    list_init(&list, 10);
    list_push(&list, 0, 10);
    list_push(&list, 1, 20);
    list_push(&list, 2, 30);
    list_push(&list, 3, 40);
    list_push(&list, 4, 50);
    list_push(&list, 4, 55);
    list_push(&list, 4, 60);
    list_push(&list, 4, 65);
    list_push(&list, 4, 70);
    list_push(&list, 4, 75);
    list_push(&list, 4, 80);
    list_push(&list, 4, 85);
    list_push(&list, 4, 90);
    list_push(&list, 4, 95);
    list_push(&list, 4, 100);
    list_push(&list, 4, 105);
    list_push(&list, 4, 110);
    list_push(&list, 4, 115);
    list_push(&list, 4, 120);
    list_push_tail(&list, 125);
    list_pop(&list, 2);
    list_pop(&list, 3);
    list_pop(&list, 4);
    list_pop(&list, 5);
    list_pop(&list, 6);
    list_pop(&list, 7);
    list_pop(&list, 8);
    list_pop(&list, 9);
    list_pop(&list, 10);
    list_pop(&list, 11);
    list_pop(&list, 12);
    list_pop(&list, 13);
    list_pop(&list, 14);
    list_pop(&list, 15);
    list_pop(&list, 16);
    list_pop(&list, 17);

    //printf("func:list_node_num_by_data\tdata:100\tfunc:%d\n", list_node_num_by_data(&list, 100));
    //printf("func:list_index_by_data\t\tdata:100\tfunc:%d\n", list_index_by_data(&list, 100));
    //printf("func:data_by_index\t\tindex:14\tfunc:%d\n", list_data_by_index(&list, 14));
    //printf("func:list_data_by_node_num\tnode:9\tfunc:%d\n", list_data_by_node_num(&list, 9));

    list_print(&list);
    list_delete(&list);
    return 0;
}
