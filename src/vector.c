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

// Condition ? value_if_true : value_if_false ternary operator

/*
 (A)
 So if length and capacity are the same, multiply capacity by 2, then
 realloc handles copying any existing data to the new block automatically.
 Then copy the new element into the first empty slot. If the allocation
 (tmp) fails, return -1 without touching v->data. If it succeeds, tmp is
 assigned as the new data and new_cap is set as the new capacity. If
 capacity and length are not equal, skip the resize and just copy the new
 data into the next empty slot, increment length, and return 0. elem is a
 pointer to the element the caller wants to add.
*/

/*
 (B)
 If you rest one in length in the next vector_push, it gets deleted without
 doing anything
*/
