#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <string.h>

/**
 * @brief Generic dynamic array that grows by doubling capacity when full.
 *
 * Stores elements as raw bytes using a void pointer and an element-size field,
 * allowing any data type (int, float, structs, pointers) to be used without
 * changing the library code.
 */
typedef struct {
    void *data;        /**< Heap-allocated buffer holding all elements */
    size_t capacity;   /**< Total number of elements the buffer can currently hold */
    size_t length;     /**< Number of elements actually stored (always <= capacity) */
    size_t elem_size;  /**< Size in bytes of a single element, set once at init */
} vector;

/**
 * @brief Initialise a vector to hold elements of a given byte size.
 *
 * Sets all fields to zero/NULL and stores elem_size for later arithmetic.
 * The internal buffer is allocated lazily on the first push.
 *
 * @param v         Pointer to the vector to initialise.
 * @param elem_size Size of one element in bytes (e.g. sizeof(int)).
 * @return 0 on success.
 */
int vector_init(vector *v, size_t elem_size);

/**
 * @brief Append one element to the end of the vector.
 *
 * If capacity is exhausted the internal buffer is doubled (or set to 4
 * elements on the first push).  The caller's element is memcpy'd into the
 * next free slot; the vector owns its own copy afterwards.
 *
 * @param v    Pointer to the vector.
 * @param elem Pointer to the element to copy in.
 * @return 0 on success, -1 if realloc fails (the vector is left untouched).
 */
int vector_push(vector *v, void *elem);

/**
 * @brief Remove the last element from the vector.
 *
 * @param v Pointer to the vector.
 * @return 0 on success, -1 if the vector is already empty.
 */
int vector_pop(vector *v);

/**
 * @brief Return a pointer to the element at the given index.
 *
 * The returned pointer aliases the vector's internal buffer — it is valid
 * only until the next push, delete, or free that reallocates the buffer.
 *
 * @param v Pointer to the vector.
 * @param i Zero-based index.
 * @return Pointer to the element, or NULL if i is out of range.
 */
void *vector_get(vector *v, size_t i);

/**
 * @brief Remove the element at index i, shifting subsequent elements left.
 *
 * Uses memmove internally so overlapping regions within the same buffer
 * are handled safely.
 *
 * @param v Pointer to the vector.
 * @param i Zero-based index.
 * @return 0 on success, -1 if i is out of range.
 */
int vector_delete(vector *v, size_t i);

/**
 * @brief Print every element as a sequence of raw bytes.
 *
 * Useful for debugging memory layout; for human-readable output use
 * a callback-based print instead.
 *
 * @param v Pointer to the vector.
 */
void vector_print(vector *v);

/**
 * @brief Release the internal buffer and reset all fields to zero.
 *
 * After this call the vector can be re-initialised with vector_init.
 *
 * @param v Pointer to the vector.
 */
void vector_free(vector *v);

#endif
