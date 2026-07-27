#include "../inc/vector.h"

int run(void) {
    vector v;

    vector_init(&v, sizeof(double));

    for (double x = 1.0; x <= 5.0; x += 1.0)
        vector_push(&v, &x);

    vector_pop(&v);

    double *val = vector_get(&v, 0);
    if (val)
        printf("Element = %f\n", *val);

    vector_delete(&v, 1);

    vector_print(&v);

    vector_free(&v);

    return 0;
}
int main(void) { return run(); }
