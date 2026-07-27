# Vector

A generic dynamic array (like C++ `std::vector`) implemented in C.

## What it does

Provides a type-agnostic, heap-allocated dynamic array that grows automatically by doubling capacity when full. The library uses a `void*` + element-size pattern to store any data type — `int`, `float`, `double`, `char`, structs, or pointers.

## Requirements

- C17
- Clang (or GCC with minor Makefile adjustment)
- [Criterion](https://github.com/Snaipe/Criterion) for tests only

## Build

```bash
make          # build the example program (bin/vector)
make test     # build and run the test suite (bin/test_vector)
make clean    # remove build artifacts
```

## Usage

```c
#include "vector.h"

int main(void) {
    vector v;
    vector_init(&v, sizeof(int));

    for (int i = 10; i <= 50; i += 10)
        vector_push(&v, &i);

    vector_print(&v);

    vector_pop(&v);

    int *val = vector_get(&v, 0);
    if (val) printf("First element: %d\n", *val);

    vector_delete(&v, 1);

    vector_free(&v);
    return 0;
}
```

## API

| Function | Signature | Description |
| ---------- | ----------- | ------------- |
| `init` | `int vector_init(vector *v, size_t elem_size)` | Initialise the vector with the byte-size of one element |
| `push` | `int vector_push(vector *v, void *elem)` | Copy one element onto the end; doubles capacity if full |
| `pop` | `int vector_pop(vector *v)` | Remove the last element |
| `get` | `void *vector_get(vector *v, size_t i)` | Return a pointer to the element at index `i`, or `NULL` |
| `delete` | `int vector_delete(vector *v, size_t i)` | Remove element at index `i`, shifting the remaining elements left |
| `print` | `void vector_print(vector *v)` | Dump every element as raw bytes |
| `free` | `void vector_free(vector *v)` | Release the internal buffer and zero the struct |

All `int`-returning functions return `0` on success and `-1` on error.

## Architecture

```
vector.h   — public struct and function declarations
vector.c   — implementation (malloc/realloc/free, memcpy/memmove)
main.c     — example usage
tests/     — Criterion test suite
Makefile   — builds the example binary and test runner
```

## Destructor hook

When storing *owning pointers* (values from `malloc`), the vector cannot know how
to free the data they point to.  Set `v.dtor` to a cleanup function and it will be
called on every element before it is removed by `pop`, `delete`, or `free`:

```c
void free_ptr(void *elem) { free(*(void **)elem); }

int main(void) {
    vector v;
    vector_init(&v, sizeof(char *));
    v.dtor = free_ptr;                   // register the destructor

    char *s = malloc(6);
    strcpy(s, "hello");
    vector_push(&v, &s);                 // push the pointer, not the string

    vector_pop(&v);                      // dtor calls free(s)
    vector_free(&v);                     // dtor called on remaining elements

    return 0;
}
```

For plain types (`int`, `float`, `double`, structs without heap pointers) this is
unnecessary — leave `v.dtor` at its default `NULL` and nothing happens.

The struct tracks five fields:

| Field | Purpose |
| ------- | --------- |
| `void *data` | Pointer to the heap-allocated buffer |
| `size_t capacity` | Total slots currently allocated |
| `size_t length` | Slots actually in use |
| `size_t elem_size` | Byte-width of one element (set at `init`) |
| `void (*dtor)(void *)` | Optional destructor callback, `NULL` by default |

## License

MIT
