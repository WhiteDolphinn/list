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
    list_push(&list, 2, 35);
    list_pop(&list, 2);
    list_delete(&list);
    return 0;
}
