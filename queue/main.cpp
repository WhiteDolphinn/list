#include <stdio.h>
#include "queue_soft.h"

int main()
{
    struct queue queue;
    queue_init(&queue);

    for(int i = 0; i < 23; i++)
    {
        queue_push(&queue, i);
        if(i % 2 == 1)
            queue_pop(&queue);
    }

    queue_delete(&queue);
    return 0;
}
