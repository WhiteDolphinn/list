#include "soft_list.h"
#include <stdlib.h>

void list_init(struct list* list)
{
    list->size = 10;
    list->head = (struct node*)calloc(list->size, sizeof(struct node));
}

void list_delete(struct list* list)
{
    list->size = 0;
    free(list->head);
}
