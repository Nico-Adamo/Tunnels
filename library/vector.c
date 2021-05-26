#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

vector_t vec_init(double x, double y) {
    vector_t v;
    v.x = x;
    v.y = y;
    return v;
}

const vector_t VEC_ZERO = {.x = 0, .y = 0};

vector_t vec_add(vector_t v1, vector_t v2) {
    return vec_init(v1.x + v2.x, v1.y + v2.y);
}

vector_t vec_subtract(vector_t v1, vector_t v2) {
    return vec_init(v1.x - v2.x, v1.y - v2.y);
}

vector_t vec_negate(vector_t v1) {
    return vec_init(-v1.x, -v1.y);
}

vector_t vec_multiply(double scalar, vector_t v) {
    return vec_init(v.x * scalar, v.y * scalar);
}

double vec_dot(vector_t v1, vector_t v2) {
    return (v1.x * v2.x) + (v1.y * v2.y);
}

double vec_cross(vector_t v1, vector_t v2) {
    return (v1.x * v2.y) - (v1.y * v2.x);
}

vector_t vec_rotate(vector_t v, double angle) {
    double x = v.x * cos(angle) - v.y * sin(angle);
    double y = v.x * sin(angle) + v.y * cos(angle);
    return vec_init(x, y);
}

vector_t vec_unit(vector_t v) {
    double magnitude = sqrt(pow(v.x, 2) + pow(v.y, 2));
    vector_t unit = {
        .x = v.x / magnitude,
        .y = v.y / magnitude
    };
    return unit;
}

double vec_distance(vector_t v1, vector_t v2) {
    return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
}

vector_t vec_find_direction(vector_t vec1, vector_t vec2) {
    return vec_unit(vec_subtract(vec1, vec2));
}
