#include "../inc/vector.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int vector_init(vector *v, size_t elem_size) {
    v->data = NULL;
    v->capacity = 0; // Total number of elements the allocated memory can hold
    v->length = 0;   // Actual usage of the allocated memory
    v->elem_size = elem_size;
    v->dtor = NULL;

    return 0;
}

int vector_push(vector *v, void *elem) { //(A)
    if (v->length == v->capacity) {
        size_t new_cap = v->capacity == 0 ? 4 : v->capacity * 2;
        void *tmp = realloc(v->data, new_cap * v->elem_size);
        if (!tmp)
            return -1;
        v->data = tmp;
        v->capacity = new_cap;
    }
    memcpy((char *)v->data + v->length * v->elem_size, elem, v->elem_size);
    v->length++;
    return 0;
}

int vector_pop(vector *v) {
    if (v->length == 0) {
        return -1;
    }
    if (v->dtor) {
        void *elem = (char *)v->data + (v->length - 1) * v->elem_size;
        v->dtor(elem);
    }
    v->length--; //(B)
    return 0;
}

void *vector_get(vector *v, size_t i) {
    if (i >= v->length) {
        return NULL;
    } else {
        return (void *)((char *)v->data + i * v->elem_size);
    }
}

void free_ptr(void *elem) { free(*(void **)elem); }

int vector_delete(vector *v, size_t i) {
    if (i >= v->length) {
        return -1;
    }
    if (v->dtor) {
        void *elem = (char *)v->data + i * v->elem_size;
        v->dtor(elem);
    }
    memmove((char *)v->data + i * v->elem_size,
            (char *)v->data + (i + 1) * v->elem_size,
            (v->length - i - 1) * v->elem_size); //(C)
    v->length--;
    return 0;
}

void vector_print_with(vector *v, void (*print_elem)(void *)) {
    printf("[\n");
    for (size_t i = 0; i < v->length; i++) {
        void *elem = (char *)v->data + i * v->elem_size;
        printf("  ");
        print_elem(elem);
        printf("\n");
    }
    printf("]\n");
}

void vector_print(vector *v) {
    for (size_t i = 0; v->length > i; i++) {
        unsigned char *byte = (unsigned char *)v->data + i * v->elem_size;
        printf("[ ");
        for (size_t j = 0; v->elem_size > j; j++) {
            printf(" %d ", byte[j]);
        }
        printf(" ]\n");
    }
}

void vector_free(vector *v) {

    if (v->dtor) {
        for (size_t i = 0; i < v->length; i++)
            v->dtor((char *)v->data + i * v->elem_size);
    }
    free(v->data);
    v->data = NULL;
    v->capacity = 0;
    v->length = 0;
    v->elem_size = 0;
}
