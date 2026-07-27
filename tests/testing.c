#include "../inc/vector.h"
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>

Test(Vector, push_into_vector) {
    vector v;
    int x = 44;
    vector_init(&v, sizeof(int));
    cr_assert_eq(vector_push(&v, &x), 0);
    cr_assert_eq(v.length, 1);
    cr_assert_eq(v.capacity, 4);
    vector_print(&v);
    vector_free(&v);
}

Test(Vector, push_five_num) {
    vector v;
    int nums[] = {22, 22, 33, 41, 94};
    vector_init(&v, sizeof(int));
    for (int i = 0; i < 5; i++) {
        vector_push(&v, &nums[i]);
    }
    cr_assert_eq(v.length, 5);
    cr_assert_eq(v.capacity, 8);
    for (int i = 0; i < 5; i++) {
        int *val = vector_get(&v, i);
        cr_assert_not_null(val);
        cr_assert_eq(*val, nums[i]);
    }
    vector_print(&v);
    vector_free(&v);
}

Test(Vector, push_six_char) {
    vector v;
    char word[] = {'H', 'e', 'l', 'l', 'o', '!'};
    vector_init(&v, sizeof(char));
    for (int i = 0; i < 6; i++) {
        vector_push(&v, &word[i]);
    }
    cr_assert_eq(v.length, 6);
    cr_assert_eq(v.capacity, 8);
    for (int i = 0; i < 6; i++) {
        char *val = vector_get(&v, i);
        cr_assert_not_null(val);
        cr_assert_eq(*val, word[i]);
    }
    vector_print(&v);
    vector_free(&v);
}

Test(Vector, pop_on_empty) {
    vector v;
    vector_init(&v, sizeof(int));
    cr_assert_eq(v.length, 0);
    cr_assert_eq(vector_pop(&v), -1);
    cr_assert_eq(v.length, 0);
    vector_free(&v);
}

Test(Vector, pop_single) {
    vector v;
    vector_init(&v, sizeof(int));
    int x = 88;
    vector_push(&v, &x);
    vector_pop(&v);
    cr_assert_eq(v.length, 0);
    cr_assert_null(vector_get(&v, 0));
    vector_free(&v);
}

Test(Vector, delete_out_of_bounds) {
    vector v;
    vector_init(&v, sizeof(int));
    int x = 44;
    vector_push(&v, &x);
    vector_delete(&v, 3);
    cr_assert_eq(v.length, 1);
    int *val = vector_get(&v, 0);
    cr_assert_not_null(val);
    cr_assert_eq(*val, 44);
    vector_free(&v);
}

Test(Vector, get_out_bounds) {
    vector v;
    vector_init(&v, sizeof(int));
    int x = 898;
    vector_push(&v, &x);
    void *val = vector_get(&v, 2);
    cr_assert_null(val);
    vector_free(&v);
}

Test(Vector, delete_first) {
    vector v;
    vector_init(&v, sizeof(int));
    int nums[] = {22, 22, 33, 41, 94};
    for (int i = 0; i < 5; i++) {
        vector_push(&v, &nums[i]);
    }
    vector_delete(&v, 0);
    cr_assert_eq(v.length, 4);
    int *val = vector_get(&v, 0);
    cr_assert_eq(*val, 22);
    vector_free(&v);
}

Test(Vector, delete_middle) {
    vector v;
    vector_init(&v, sizeof(int));
    int nums[] = {22, 22, 33, 41, 94};
    for (int i = 0; i < 5; i++) {
        vector_push(&v, &nums[i]);
    }
    vector_delete(&v, 1);
    cr_assert_eq(v.length, 4);
    int *val = vector_get(&v, 2);
    cr_assert_not_null(val);
    cr_assert_eq(*val, 41);
    vector_free(&v);
}

Test(Vector, delete_last) {
    vector v;
    vector_init(&v, sizeof(int));
    int nums[] = {22, 22, 33, 41, 94};
    for (int i = 0; i < 5; i++) {
        vector_push(&v, &nums[i]);
    }
    vector_delete(&v, 4);
    cr_assert_eq(v.length, 4);
    int *val = vector_get(&v, 3);
    cr_assert_not_null(val);
    cr_assert_eq(*val, 41);
    vector_free(&v);
}

Test(Vector, push_many) {
    vector v;
    vector_init(&v, sizeof(int));
    for (int i = 0; i < 1000; i++)
        vector_push(&v, &i);
    cr_assert_eq(v.length, 1000);
    cr_assert_eq(v.capacity, 1024);
    int *first = vector_get(&v, 0);
    cr_assert_not_null(first);
    cr_assert_eq(*first, 0);
    int *last = vector_get(&v, 999);
    cr_assert_not_null(last);
    cr_assert_eq(*last, 999);
    vector_free(&v);
}

Test(Vector, push_double) {
    vector v;
    vector_init(&v, sizeof(double));
    double nums[] = {22.001334, 22.001334, 33.001334, 41.001334, 94.001334};
    for (int i = 0; i < 5; i++) {
        vector_push(&v, &nums[i]);
    }
    cr_assert_eq(v.length, 5);
    cr_assert_eq(v.capacity * v.elem_size, 64);
    vector_free(&v);
}

Test(Vector, push_pop_push) {
    vector v;
    vector_init(&v, sizeof(int));
    for (int i = 10; i <= 30; i += 10)
        vector_push(&v, &i); // [10, 20, 30], length=3
    vector_pop(&v);          // [10, 20], length=2
    for (int i = 40; i <= 50; i += 10)
        vector_push(&v, &i); // [10, 20, 40, 50], length=4
    cr_assert_eq(v.length, 4);
    int *val = vector_get(&v, 2);
    cr_assert_not_null(val);
    cr_assert_eq(*val, 40); // popped 30, pushed 40 at index 2
    vector_free(&v);
}

Test(Vector, push_delete_push) {
    vector v;
    vector_init(&v, sizeof(int));
    for (int i = 10; i <= 40; i += 10)
        vector_push(&v, &i); // [10, 20, 30, 40], length=4
    vector_delete(&v, 1);    // [10, 30, 40], length=3
    for (int i = 50; i <= 60; i += 10)
        vector_push(&v, &i); // [10, 30, 40, 50, 60], length=5

    cr_assert_eq(v.length, 5);
    int *val = vector_get(&v, 1);
    cr_assert_not_null(val);
    cr_assert_eq(*val, 30); // 30 shifted from index 2 to 1
    vector_free(&v);
}
