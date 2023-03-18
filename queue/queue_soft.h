#ifndef QUEUE_SOFT_H
#define QUEUE_SOFT_H

#include <stdio.h>
//#define QUEUE_SIZE 64 //обязательно степень двойки
#define POISON 14888841
//#define MASK (QUEUE_SIZE - 1)
//#define BLOCK_QUEUE_CHECK

struct queue{
    int* data;
    unsigned int head;
    unsigned int tail;
    unsigned int size;
    unsigned int mask;
    bool block_push;
    bool block_pop;
};

void queue_init(struct queue* queue, unsigned int queue_size);
void queue_delete(struct queue* queue);
void queue_push(struct queue* queue, int i);
int queue_pop(struct queue* queue);
void queue_check(FILE* file, struct queue* queue, const char* filename, const char* function_name, int line);
void queue_resize(struct queue* queue, unsigned int new_queue_size);

#endif
