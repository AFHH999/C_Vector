#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <string.h>

typedef struct {
    void *data;
    size_t capacity;
    size_t length;
    size_t elem_size;
} vector;

int vector_init(vector *v, size_t elem_size);
int vector_push(vector *v, void *elem);

#endif
