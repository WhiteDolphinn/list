#include "soft_list.h"
#include <stdlib.h>

void list_init(struct list* list)
{
    list->size = 10;
    list->head = (struct node*)calloc(list->size, sizeof(struct node));

    for(int i = 0; i < list->size; i++)
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
}
