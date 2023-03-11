#include "soft_list.h"
#include "queue/log.h"

int main()
{
    struct list list;
    list_init(&list);
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
    list_push(&list, 4, 125);
    list_pop(&list, 2);
    list_print(&list);
    list_delete(&list);
    return 0;
}
