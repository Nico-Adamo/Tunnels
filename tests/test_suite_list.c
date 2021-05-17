#include "list.h"
// #include "test_util.h"
// #include "polygon.h"
// #include "body.h"
// #include <assert.h>
// #include <stdlib.h>
// #include <math.h>

// list_t *star(vector_t center, int sides, double radius, double angle) {
//     // creating reference vectors
//     vector_t v = (vector_t) {radius, 0.0};
//     vector_t v_in = (vector_t) {radius / 2.5, 0.0};
//     v_in = vec_rotate(v_in, (2 * M_PI) / (2 * sides));

//     list_t *points = list_init(sides * 2, free);

//     // generating star points
//     for (int i = 0; i < sides; i++) {
//         v = vec_rotate(v, 2 * M_PI / sides);
//         vector_t *v_i = malloc(sizeof(vector_t));
//         v_i->x = v.x;
//         v_i->y = v.y;
//         v_in = vec_rotate(v_in, 2 * M_PI / sides);
//         vector_t *v_in_i = malloc(sizeof(vector_t));
//         v_in_i->x = v_in.x;
//         v_in_i->y = v_in.y;
//         list_add(points, v_i);
//         list_add(points, v_in_i);
//     }
//     polygon_translate(points, center);
//     polygon_rotate(points, angle, center);
//     return points;
// }

// void test_list_size0() {
//     list_t *l = list_init(0, free);
//     assert(list_size(l) == 0);
//     list_free(l);
// }

// void test_list_size1() {
//     list_t *l = list_init(1,free);
//     assert(list_size(l) == 0);

//     // Add
//     list_t *obj1 = star((vector_t) {0,0}, 5, 50, 0);
//     hsl_color_t col1 = {.h = 50, .s = 1, .l = .72};
//     body_t *v1 = object_init(obj1, (vector_t) {0, 0}, col1);
//     list_add(l, v1);
//     assert(list_size(l) == 1);

//     // Remove back
//     assert(list_remove_back(l) == v1);
//     object_free(v1);
//     assert(list_size(l) == 0);

//     // Add again
//     list_t *obj2 = star((vector_t) {1,1}, 3, 50, 0);
//     hsl_color_t col2 = {.h = 50, .s = 1, .l = .72};
//     body_t *v2 = object_init(obj2, (vector_t) {1, 1}, col2);
//     list_add(l, v2);
//     assert(list_size(l) == 1);
//     assert(list_get(l, 0) == v2);

//     list_free(l);
// }

// void test_list_small() {
//     list_t *l = list_init(5, free);
//     assert(list_size(l) == 0);

//     // Fill partially
//     list_t *obj1 = star((vector_t) {0,0}, 5, 50, 0);
//     hsl_color_t col1 = {.h = 50, .s = 1, .l = .72};
//     body_t *v1 = object_init(obj1, (vector_t) {0, 0}, col1);
//     list_add(l, v1);
//     list_t *obj2 = star((vector_t) {1,1}, 3, 50, 0);
//     hsl_color_t col2 = {.h = 50, .s = 1, .l = .72};
//     body_t *v2 = object_init(obj2, (vector_t) {1, 1}, col2);
//     list_add(l, v2);
//     list_t *obj3 = star((vector_t) {2,2}, 4, 50, 0);
//     hsl_color_t col3 = {.h = 50, .s = 1, .l = .72};
//     body_t *v3 = object_init(obj3, (vector_t) {2, 2}, col3);
//     list_add(l, v3);
//     assert(list_size(l) == 3);
//     assert(list_get(l, 0) == v1);
//     assert(list_get(l, 1) == v2);
//     assert(list_get(l, 2) == v3);

//     // Fill to capacity
//     list_t *obj4 = star((vector_t) {3,3}, 5, 50, 0);
//     hsl_color_t col4 = {.h = 50, .s = 1, .l = .72};
//     body_t *v4 = object_init(obj4, (vector_t) {3, 3}, col4);
//     list_add(l, v4);
//     list_t *obj5 = star((vector_t) {4,4}, 5, 50, 0);
//     hsl_color_t col5 = {.h = 50, .s = 1, .l = .72};
//     body_t *v5 = object_init(obj5, (vector_t) {4, 4}, col5);
//     list_add(l, v5);
//     assert(list_size(l) == 5);
//     assert(list_get(l, 3) == v4);
//     assert(list_get(l, 4) == v5);

//     // Remove some
//     body_t *v5_r = list_remove_back(l);
//     assert(v5_r == v5);
//     object_free(v5_r);
//     body_t *v1_r = list_remove_front(l);
//     assert(v1_r == v1);
//     object_free(v1_r);
//     assert(list_size(l) == 3);
// }

// #define LARGE_SIZE 100

// // Get/set elements in large list
// void test_list_large_get_set() {
//     list_t *l = list_init(LARGE_SIZE, free);

//     // Add to capacity
//     list_t *obj;
//     hsl_color_t col;
//     body_t *v;
//     for (size_t i = 0; i < LARGE_SIZE; i++) {
//         obj = star((vector_t) {i,i}, 5, 50, 0);
//         col = (hsl_color_t) {.h = 50, .s = 1, .l = .72};
//         v = object_init(obj, (vector_t) {i, i}, col);
//         list_add(l, v);
//         assert(list_get(l, i) == v);
//     }
//     list_free(l);
// }

// // Add/remove elements from a large list
// void test_list_large_add_remove() {
//     list_t *l = list_init(LARGE_SIZE, free);

//     // Add to capacity
//     list_t *obj;
//     hsl_color_t col;
//     body_t *v;
//     for (size_t i = 0; i < LARGE_SIZE; i++) {
//         obj = star((vector_t) {i,i}, 5, 50, 0);
//         col = (hsl_color_t) {.h = 50, .s = 1, .l = .72};
//         v = object_init(obj, (vector_t) {i, i}, col);
//         list_add(l, v);
//     }

//     // Remove back
//     for (size_t i = 1; i <= LARGE_SIZE; i++) {
//         body_t *v_r = list_get(l, list_size(l) - 1);
//         body_t *v_r_2;
//         if (list_size(l) > 1) {
//             v_r_2 = list_get(l, list_size(l) - 2);
//         }
//         v = list_remove_back(l);
//         assert(v_r == v);
//         if (list_size(l) > 0) {
//             assert(v_r_2 == list_get(l, list_size(l) - 1));
//         }
//         object_free(v);
//     }

//     // Add to capacity again
//     for (size_t i = 0; i < LARGE_SIZE; i++) {
//         obj = star((vector_t) {i,i}, 5, 50, 0);
//         col = (hsl_color_t) {.h = 50, .s = 1, .l = .72};
//         v = object_init(obj, (vector_t) {i, i}, col);
//         list_add(l, v);
//     }

//     // Remove front
//     for (size_t i = 1; i <= LARGE_SIZE; i++) {
//         body_t *v_f = list_get(l, 0);
//         body_t *v_f_2;
//         if (list_size(l) > 1) {
//             v_f_2 = list_get(l, 1);
//         }
//         v = list_remove_front(l);
//         assert(v_f == v);
//         if (list_size(l) > 0) {
//             assert(v_f_2 == list_get(l, 0));
//         }
//         object_free(v);
//     }

//     // Add to capacity
//     for (size_t i = 0; i < LARGE_SIZE; i++) {
//         obj = star((vector_t) {i,i}, 5, 50, 0);
//         col = (hsl_color_t) {.h = 50, .s = 1, .l = .72};
//         v = object_init(obj, (vector_t) {i, i}, col);
//         list_add(l, v);
//         assert(list_get(l, i) == v);
//     }
//     list_free(l);
// }

// void test_full_add() {
//     const size_t cap = 3;
//     list_t *l = list_init(cap, free);

//     // Fill list
//     for (size_t i = 0; i < cap; i++) {
//         list_add(l, malloc(sizeof(body_t)));
//     }

//     // Try adding to the full list
//     list_add(l, malloc(sizeof(body_t)));
//     assert(list_capacity(l) == 2 * cap);

//     list_free(l);
// }

// void test_resize() {
//     const size_t cap = 3;
//     list_t *l = list_init(3, free);
//     hsl_color_t col = {.h = 50, .s = 1, .l = .72};

//     // Fill list
//     list_t *obj1 = star((vector_t) {0,0}, 5, 50, 0);
//     body_t *v1 = object_init(obj1, (vector_t) {0, 0}, col);
//     list_add(l, v1);


//     list_t *obj2 = star((vector_t) {1,1}, 3, 50, 0);
//     body_t *v2 = object_init(obj2, (vector_t) {1, 1}, col);
//     list_add(l, v2);

//     list_t *obj3 = star((vector_t) {2,2}, 4, 50, 0);
//     body_t *v3 = object_init(obj3, (vector_t) {2, 2}, col);
//     list_add(l, v3);

//     assert(list_size(l) == 3);
//     assert(list_capacity(l) == 3);
//     assert(list_get(l, 0) == v1);
//     assert(list_get(l, 1) == v2);
//     assert(list_get(l, 2) == v3);

//     // Add past capacity
//     list_t *obj4 = star((vector_t) {3,3}, 5, 50, 0);
//     body_t *v4 = object_init(obj4, (vector_t) {0, 0}, col);
//     list_add(l, v4);

//     assert(list_get(l, 3) == v4);
//     assert(object_get(list_get(l, 3)) == obj4);
//     assert(list_capacity(l) == 2 * cap);

//     list_free(l);
// }

// void remove_back_from_empty(void *l) {
//     list_remove_back((list_t *) l);
// }

// void test_empty_remove_back() {
//     const size_t size = 100;
//     list_t *l = list_init(size, free);
//     body_t *v;

//     // Fill list with copies of v, then remove them all
//     list_t *obj = star((vector_t) {0,0}, 5, 50, 0);
//     hsl_color_t col = {.h = 50, .s = 1, .l = .72};
//     v = object_init(obj, (vector_t) {0, 0}, col);
//     for (size_t i = 0; i < size; i++) {
//         body_t *list_v = malloc(sizeof(*list_v));
//         list_v = v;
//         list_add(l, list_v);
//     }
//     for (size_t i = 0; i < size; i++) {
//         body_t *v_r;
//         v_r = list_get(l, list_size(l) - 1);
//         v = list_remove_back(l);
//         assert(v_r = v);
//     }
//     object_free(v);
//     assert(test_assert_fail(remove_back_from_empty, l));

//     list_free(l);
// }

// void add_null(void *l) {
//     list_add(l, NULL);
// }

// void test_null_values() {
//     list_t *l = list_init(1, free);
//     assert(test_assert_fail(add_null, l));
//     list_free(l);
// }

int main(int argc, char *argv[]) {
    // // Run all tests if there are no command-line arguments
    // bool all_tests = argc == 1;
    // // Read test name from file
    // char testname[100];
    // if (!all_tests) {
    //     read_testname(argv[1], testname, sizeof(testname));
    // }

    // DO_TEST(test_list_size0)
    // DO_TEST(test_list_size1)
    // DO_TEST(test_list_small)
    // DO_TEST(test_list_large_get_set)
    // DO_TEST(test_list_large_add_remove)
    // DO_TEST(test_full_add)
    // DO_TEST(test_resize)
    // DO_TEST(test_empty_remove_back)
    // DO_TEST(test_null_values)

    // puts("list_test PASS");
    
    return 0;
}
