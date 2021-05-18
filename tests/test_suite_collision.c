// #include <assert.h>
// #include <math.h>
// #include <stdlib.h>

// #include "forces.h"
// #include "test_util.h"
// #include "collision.h"


// list_t *make_shape() {
//     list_t *shape = list_init(4, free);
//     vector_t *v = malloc(sizeof(*v));
//     *v = (vector_t){-1, -1};
//     list_add(shape, v);
//     v = malloc(sizeof(*v));
//     *v = (vector_t){+1, -1};
//     list_add(shape, v);
//     v = malloc(sizeof(*v));
//     *v = (vector_t){+1, +1};
//     list_add(shape, v);
//     v = malloc(sizeof(*v));
//     *v = (vector_t){-1, +1};
//     list_add(shape, v);
//     return shape;
// }

// void _test_static_collision(vector_t pos1, vector_t pos2, bool shouldCollide) {

//     const double DT = 1e-6;
//     const int STEPS = 1000;

//     scene_t *scene = scene_init();
//     body_t *mass1 = body_init(make_shape(), INFINITY, (rgb_color_t){0, 0, 1});
//     body_set_centroid(mass1, pos1);
//     scene_add_body(scene, mass1);
//     body_t *mass2 = body_init(make_shape(), INFINITY, (rgb_color_t){1, 0, 0});
//     body_set_centroid(mass2, pos2);
//     scene_add_body(scene, mass2);

//     for (int i = 0; i < STEPS; i++) {
//         assert(find_collision(body_get_shape(mass1), body_get_shape(mass2)).collided == shouldCollide);
//         scene_tick(scene, DT);
//     }

//     scene_free(scene);
// }

// void test_static_collisions() {
//     _test_static_collision((vector_t) {100, 0}, (vector_t) {102, 0}, true); // Tangent collision
//     _test_static_collision((vector_t) {100, 0}, (vector_t) {103, 0}, false); // Not colliding
//     _test_static_collision((vector_t) {100, 0}, (vector_t) {100, 0}, true); // Overlapping collision
// }

// void test_moving_collision() {

//     const double DT = 1e-5;
//     const int STEPS = 1000000;
//     const double velocity = 10;

//     scene_t *scene = scene_init();
//     body_t *mass1 = body_init(make_shape(), INFINITY, (rgb_color_t){0, 0, 1});
//     body_set_centroid(mass1, (vector_t) {0, 0});
//     body_set_velocity(mass1, (vector_t) {velocity, 0});
//     scene_add_body(scene, mass1);
//     body_t *mass2 = body_init(make_shape(), INFINITY, (rgb_color_t){1, 0, 0});
//     body_set_centroid(mass2, (vector_t) {100, 0});
//     body_set_velocity(mass1, (vector_t) {-velocity, 0});
//     scene_add_body(scene, mass2);

//     for (int i = 0; i < STEPS; i++) {
//         assert(find_collision(body_get_shape(mass1), body_get_shape(mass2)).collided == fabs(body_get_centroid(mass1).x - body_get_centroid(mass2).x) <= 2);
//         scene_tick(scene, DT);
//     }

//     scene_free(scene);
// }

int main(int argc, char *argv[]) {
    // // Run all tests if there are no command-line arguments
    // bool all_tests = argc == 1;
    // // Read test name from file
    // char testname[100];
    // if (!all_tests) {
    //     read_testname(argv[1], testname, sizeof(testname));
    // }

    // DO_TEST(test_static_collisions)
    // DO_TEST(test_moving_collision)

    // puts("collision_test PASS");
    return 0;
}
