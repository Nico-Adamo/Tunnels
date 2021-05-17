#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "forces.h"
#include "test_util.h"


double spring_potential(double k, body_t *body) {
    vector_t displacement = body_get_centroid(body);
    double x = displacement.x;
    return (.5 * k * pow(x, 2));
}
double kinetic_energy(body_t *body) {
    vector_t v = body_get_velocity(body);
    return body_get_mass(body) * vec_dot(v, v) / 2;
}

list_t *make_shape() {
    list_t *shape = list_init(4, free);
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t){-1, -1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t){+1, -1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t){+1, +1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t){-1, +1};
    list_add(shape, v);
    return shape;
}

void test_spring_energy_conservation() {
    const double M = 10;
    const double K = 2;
    const double A = 3;
    const double DT = 1e-6;
    const int STEPS = 1000000;
    scene_t *scene = scene_init();
    body_t *mass = body_init(make_shape(), M, (rgb_color_t){0, 0, 0});
    body_set_centroid(mass, (vector_t){A, 0});
    scene_add_body(scene, mass);
    body_t *anchor = body_init(make_shape(), INFINITY, (rgb_color_t){0, 0, 0});
    scene_add_body(scene, anchor);
    create_spring(scene, K, mass, anchor);
    
    double initial_energy = spring_potential(K, mass);

    for (int i = 0; i < STEPS; i++) {
        double energy = spring_potential(K, mass) + kinetic_energy(mass);
        //printf("%f; %f\n", initial_energy, energy);
        assert(within(1e-4, energy / initial_energy, 1));
        scene_tick(scene, DT);
    }

    scene_free(scene);
}

void test_reasonable_gravitational_forces() {
    const double M1 = 10, M2 = 10;
    const double G = 1e3;
    const double DT = 1e-6;
    const int STEPS = 1000000;
    scene_t *scene = scene_init();
    body_t *mass1 = body_init(make_shape(), M1, (rgb_color_t){0, 0, 0});
    scene_add_body(scene, mass1);
    body_t *mass2 = body_init(make_shape(), M2, (rgb_color_t){0, 0, 0});
    body_set_centroid(mass2, (vector_t){10, 20});
    scene_add_body(scene, mass2);
    create_newtonian_gravity(scene, G, mass1, mass2);
    for (int i = 0; i < STEPS; i++) {
        // Check that two bodies of the same mass have the same magnitude of velocity
        assert(vec_isclose(vec_add(body_get_velocity(mass1), body_get_velocity(mass2)), VEC_ZERO));
        // Check that forces do not blow up at small distances
        assert(kinetic_energy(mass1) < 1e6);
        assert(kinetic_energy(mass2) < 1e6);
        scene_tick(scene, DT);
    }
    scene_free(scene);
}

void test_drag() {
    const double M1 = 10;
    const double DT = 1e-6;
    const int STEPS = 1000000;
    scene_t *scene = scene_init();
    body_t *mass = body_init(make_shape(), M1, (rgb_color_t){0, 0, 0});
    body_set_velocity(mass, (vector_t){.x = 200, .y = 0});
    scene_add_body(scene, mass);
    create_drag(scene, .5, mass);
    vector_t temp_velocity = {.x = 200, .y = 0};
    for (int i = 0; i < STEPS; i++) {
        scene_tick(scene, DT);
        // Check that drag decreases velocity
        assert(body_get_velocity(mass).x < temp_velocity.x);
        temp_velocity = body_get_velocity(mass);
        assert(body_get_velocity(mass).y == 0);
        assert(body_get_velocity(mass).x >= 0);
    }
    scene_free(scene);
}

int main(int argc, char *argv[]) {
    // Run all tests if there are no command-line arguments
    bool all_tests = argc == 1;
    // Read test name from file
    char testname[100];
    if (!all_tests) {
        read_testname(argv[1], testname, sizeof(testname));
    }

    DO_TEST(test_spring_energy_conservation);
    DO_TEST(test_reasonable_gravitational_forces);
    DO_TEST(test_drag);

    puts("forces_test PASS");
}

