#include <stdlib.h>
#include <string.h>
#include "queue_soft.h"
#include "log.h"

#define QUEUE_CHECK queue_check(get_log_file(".txt"), queue, __FILE__, __func__, __LINE__)

void queue_init(struct queue* queue, unsigned int queue_size)
{
    queue->head = 0;
    queue->tail = 0;
    queue->size = queue_size;
    queue->block_push = false;
    queue->block_pop = true;
    queue->data = (int*)calloc(queue->size, sizeof(int));

    for(unsigned int i = 0; i < queue->size; i++)
        queue->data[i] = POISON;

   /* for(unsigned int i = 1; ; i *= 2)
    {
        if(queue->size <= i)
        {
            queue->mask = i - 1;
            break;
        }//,сделать через побитовые сдвиги
    }*/
    QUEUE_CHECK;
}

void queue_delete(struct queue* queue)
{
    QUEUE_CHECK;

    queue->head = POISON;
    queue->tail = POISON;
    queue->size = POISON;
    free(queue->data);
}

void queue_push(struct queue* queue, int i)
{
    QUEUE_CHECK;

    if(queue->block_push == false)
    {
        queue->data[queue->tail++] = i;
        /*int len_from_mask = queue->tail + queue->mask - queue->size;

        len_from_mask &= queue->mask;*/

        if(queue->tail == queue->size)
            queue->tail = 0;
    }
    QUEUE_CHECK;
}

int queue_pop(struct queue* queue)
{
    QUEUE_CHECK;

    int i = POISON;
    if(queue->block_pop == false)
    {
        i = queue->data[queue->head];
        queue->data[queue->head] = POISON;
        queue->head++;
        //queue->head &= queue->mask;
        if(queue->head == queue->size)
            queue->head = 0;
    }
    QUEUE_CHECK;
    return i;
}

#ifndef BLOCK_QUEUE_CHECK

void queue_check(FILE* file, struct queue* queue, const char* filename, const char* function_name, int line)
{
    if(file == nullptr)
    {
        printf("Error with opening of logfile\n");
        return;
    }

    fprintf(file, "========================\n");
    for(unsigned int i = 0; i < queue->size; i++)
    {
        if(queue->data[i] != POISON)
            fprintf(file, "%u:  %d\n", i, queue->data[i]);
        else
            fprintf(file, "%u: %s\n", i, "POISON");
    }
    fprintf(file, "\tFile: %s\n", filename);
    fprintf(file, "\tFunction: %s\n", function_name);
    fprintf(file, "\tLine: %d\n", line);

    queue->block_push = ((queue->head % queue->size) - ((queue->tail + 1) % queue->size) == 0);
    queue->block_pop = (queue->tail == queue->head);

    if(queue->block_push == true)
        fprintf(file, "Pushing is blocked!!!\n");

    if(queue->block_pop == true)
        fprintf(file, "Poping is blocked!!!\n");

    fprintf(file, "queue->head = %u\n", queue->head);
    fprintf(file, "queue->tail = %u\n", queue->tail);
    fprintf(file, "========================\n\n");

}

#else

void queue_check(FILE* file, struct queue* queue, const char* filename, const char* function_name, int line)
{
    queue->block_push = ((queue->head % queue->size) - ((queue->tail + 1) % queue->size) == 0);
    queue->block_pop = (queue->tail == queue->head);

    if(queue->block_push == true)
        fprintf(file, "Pushing is blocked!!!\nFile:%s\nfunc:%s\nline:%d\n", filename, function_name, line);

    if(queue->block_pop == true)
        fprintf(file, "Poping is blocked!!!\nFile:%s\nfunc:%s\nline:%d\n", filename, function_name, line);
}

#endif

bool queue_resize(struct queue* queue, unsigned int new_queue_size)//если меньше 5,
{
    if(new_queue_size > queue->size)
    {
        int* buf_data = (int*)realloc(queue->data, new_queue_size * sizeof(int));
        if(buf_data == nullptr)
            return false;

        queue->data = buf_data;

        for(unsigned int i = queue->size; i < new_queue_size; i++)
            queue->data[i] = POISON;

        queue->size = new_queue_size;

        if(queue->head >= queue->size)
            queue->head = 0;

        if(queue->tail >= queue->size)
            queue->tail = 0;

        return true;
    }
    else
    {
        int* buf_data = (int*)calloc(new_queue_size + 2, sizeof(int));
        unsigned int num_of_elems = 0;
        for(; queue->block_pop == false; num_of_elems++)
            buf_data[num_of_elems] = queue_pop(queue);

        queue->head = 0;
        queue->tail = 0;

        int* buf_ptr = (int*)realloc(queue->data, new_queue_size * sizeof(int));
        if(buf_ptr == nullptr)
            return false;

        queue->data = buf_ptr;

        for(unsigned int i = 0; i < num_of_elems; i++)
            queue->data[i] = buf_data[i];

        queue->size = new_queue_size;
        queue->tail = num_of_elems;
        queue->head = 0;

        free(buf_data);
        return true;
    }
    return true;
}

void queue_clean(struct queue* queue)
{
    for(unsigned int i = 0; i < queue->size; i++)
        queue->data[i] = POISON;

    queue->head = 0;
    queue->tail = 0;
}
