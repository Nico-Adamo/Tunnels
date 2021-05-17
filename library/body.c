#include "body.h"
#include "polygon.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct body {
    list_t *shape;
    double mass;
    rgb_color_t color;
    vector_t centroid;
    vector_t velocity;
    double rotation;
    vector_t net_force;
    vector_t net_impulse;
    bool removed;
    void *info;
    free_func_t info_freer;
} body_t;

body_t *body_init(list_t *shape, double mass, rgb_color_t color) {
    return body_init_with_info(shape, mass, color, NULL, NULL);
}

body_t *body_init_with_info(list_t *shape, double mass, rgb_color_t color, void *info, free_func_t info_freer) {
    body_t *body = malloc(sizeof(body_t));
    assert(body != NULL);

    body->shape = shape;
    body->mass = mass;
    body->color = color;
    body->net_force.x = 0;
    body->net_force.y = 0;
    body->net_impulse.x = 0;
    body->net_impulse.y = 0;
    body->removed = false;
    body->info = info;
    body->info_freer = info_freer;


    body->centroid = polygon_centroid(shape);
    body->velocity = VEC_ZERO;
    body->rotation = 0.0;

    return body;
}

void body_free(body_t *body) {
    list_free(body->shape);
    free(body);
}

list_t *body_get_shape(body_t *body) {
    list_t *shape_list = list_init(list_size(body->shape), free);
    for (size_t i = 0; i < list_size(body->shape); i++) {
        vector_t *point = list_get(body->shape, i);
        vector_t *new_point = malloc(sizeof(vector_t));
        new_point->x = point->x;
        new_point->y = point->y;
        list_add(shape_list, new_point);
    }
    return shape_list;
}

vector_t body_get_centroid(body_t *body) {
    return body->centroid;
}

vector_t body_get_velocity(body_t *body) {
    return body->velocity;
}

double body_get_mass(body_t *body) {
    return body->mass;
}

rgb_color_t body_get_color(body_t *body) {
    return body->color;
}

void *body_get_info(body_t *body) {
    return body->info;
}

void body_set_centroid(body_t *body, vector_t x) {
    vector_t translation = vec_subtract(x, body->centroid);
    body->centroid = x;
    polygon_translate(body->shape, translation);
}

void body_set_velocity(body_t *body, vector_t v) {
    body->velocity = v;
}

void body_set_color(body_t *body, rgb_color_t color) {
    body->color = color;
}

void body_set_rotation(body_t *body, double angle) {
    polygon_rotate(body->shape, angle - body->rotation, body->centroid);
    body->rotation = angle;
}

void body_rotate(body_t *body, double angle) {
    polygon_rotate(body->shape, angle, body->centroid);
    body->rotation += angle;
}

void body_add_force(body_t *body, vector_t force) {
    // F = m*a
    body->net_force.x += force.x;
    body->net_force.y += force.y;
}

void body_add_impulse(body_t *body, vector_t impulse) {
    // I = m * delta_v
    body->net_impulse.x += impulse.x;
    body->net_impulse.y += impulse.y;
}

void body_tick(body_t *body, double dt) {

    vector_t acceleration = {
        .x = body->net_force.x / body->mass,
        .y = body->net_force.y / body->mass
    };
    vector_t velocity_change = {
        .x = acceleration.x * dt + body->net_impulse.x / body->mass,
        .y = acceleration.y * dt + body->net_impulse.y / body->mass
    };
    vector_t final_velocity = {
        .x = body->velocity.x + velocity_change.x,
        .y = body->velocity.y + velocity_change.y
    };
    vector_t average_velocity = {
        .x = (final_velocity.x + body->velocity.x) / 2,
        .y = (final_velocity.y + body->velocity.y) / 2
    };
    body_set_velocity(body, final_velocity);
    body_set_centroid(body, vec_add(body->centroid, vec_multiply(dt, average_velocity)));
    body->net_force = (vector_t) {0, 0};
    body->net_impulse = (vector_t) {0, 0};
}

void body_remove(body_t *body) {
    body->removed = true;
}

bool body_is_removed(body_t *body) {
    return body->removed;
}
