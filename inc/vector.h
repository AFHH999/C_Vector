#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <string.h>

typedef struct {
    void *data;
    size_t capacity;
    size_t length;
    size_t elem_size;
    void (*dtor)(void *);
} vector;

int vector_init(vector *v, size_t elem_size);

int vector_push(vector *v, void *elem);

void free_ptr(void *elem);

int vector_pop(vector *v);

void *vector_get(vector *v, size_t i);

int vector_delete(vector *v, size_t i);

void vector_print(vector *v);

void vector_free(vector *v);

#endif
