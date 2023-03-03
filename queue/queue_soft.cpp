#include <stdlib.h>
#include <string.h>
#include "queue_soft.h"
#include "log.h"

void queue_init(struct queue* queue)
{
    queue->head = 0;
    queue->tail = 0;
    queue->block_push = false;
    queue->block_pop = true;
    queue->data = (int*)calloc(QUEUE_SIZE, sizeof(int));

    for(int i = 0; i < QUEUE_SIZE; i++)
        queue->data[i] = POISON;

    queue_check(get_log_file(), queue, __FILE__, __func__, __LINE__);
}

void queue_delete(struct queue* queue)
{
    queue_check(get_log_file(), queue, __FILE__, __func__, __LINE__);

    queue->head = POISON;
    queue->tail = POISON;
    free(queue->data);
}

void queue_push(struct queue* queue, int i)
{
    queue_check(get_log_file(), queue, __FILE__, __func__, __LINE__);

    if(queue->block_push == false)
    {
        queue->data[queue->tail++] = i;
        queue->tail &= MASK;
    }
    queue_check(get_log_file(), queue, __FILE__, __func__, __LINE__);
}

int queue_pop(struct queue* queue)
{
    queue_check(get_log_file(), queue, __FILE__, __func__, __LINE__);

    int i = POISON;
    if(queue->block_pop == false)
    {
        i = queue->data[queue->head++];
        queue->data[queue->head - 1] = POISON;
        queue->head &= MASK;
    }
    queue_check(get_log_file(), queue, __FILE__, __func__, __LINE__);
    return i;
}

#ifndef BLOCK_QUEUE_CHECK

void queue_check(FILE* file, struct queue* queue, const char* filename, const char* function_name, int line)
{
    fprintf(file, "========================\n");
    for(int i = 0; i < QUEUE_SIZE; i++)
    {
        if(queue->data[i] != (int)POISON)
            fprintf(file, "%d:  %d\n", i, queue->data[i]);
        else
            fprintf(file, "%d: %X\n", i, queue->data[i]);
    }
    fprintf(file, "\tFile: %s\n", filename);
    fprintf(file, "\tFunction: %s\n", function_name);
    fprintf(file, "\tLine: %d\n", line);

    queue->block_push = ((queue->head & MASK) - ((queue->tail + 1) & MASK) == 0);
    queue->block_pop = (queue->tail == queue->head);

    if(queue->block_push == true)
        fprintf(file, "Pushing is blocked!!!\n");

    if(queue->block_pop == true)
        fprintf(file, "Poping is blocked!!!\n");

    fprintf(file, "========================\n\n");

}

#else

void queue_check(FILE* file, struct queue* queue, const char* filename, const char* function_name, int line)
{
   queue->block_push = ((queue->head & MASK) - ((queue->tail + 1) & MASK) == 0);
    queue->block_pop = (queue->tail == queue->head);

    if(queue->block_push == true)
        fprintf(file, "Pushing is blocked!!!\n");

    if(queue->block_pop == true)
        fprintf(file, "Poping is blocked!!!\n");
}

#endif
