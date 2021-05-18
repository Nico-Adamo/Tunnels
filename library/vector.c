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

