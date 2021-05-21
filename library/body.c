#include "body.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

// TO DO: Once sprite rendering is done, include it in typedef body_t
typedef struct body {
    body_shape_t shape;
    SDL_Texture *texture;
    double mass;
    double scale;
    vector_t centroid;
    vector_t velocity;
    double rotation;
    vector_t net_force;
    vector_t net_impulse;
    bool removed;
    bool flipped;
    char *type;
    sprite_info_t info;
    free_func_t info_freer;
    vector_t direction;
} body_t;

body_t *body_init(body_shape_t shape, SDL_Texture *texture, double mass) {
    return body_init_with_info(shape, texture, mass, 0, NULL, (sprite_info_t) {0.0, 0.0, 0.0, 0.0});
}

body_t *body_init_with_info(body_shape_t shape, SDL_Texture *texture, double mass, double scale, char *type, sprite_info_t info) {
    body_t *body = malloc(sizeof(body_t));
    assert(body != NULL);

    body->shape = shape;
    body->texture = texture;
    body->mass = mass;
    body->scale = scale;
    body->flipped = false;
    body->net_force.x = 0;
    body->net_force.y = 0;
    body->net_impulse.x = 0;
    body->net_impulse.y = 0;
    body->removed = false;
    body->type = type;
    body->info = info;
    body->info_freer = free;
    body->direction.x = 1;
    body->direction.y = 0;


    body->centroid.x = shape.hitbox.x + shape.hitbox.w / 2;
    body->centroid.y = shape.hitbox.y + shape.hitbox.h / 2;
    body->velocity = VEC_ZERO;
    body->rotation = 0.0;

    return body;
}

// Texture??
void body_free(body_t *body) {
    free(body);
}

sprite_info_t body_get_sprite_info(body_t *body) {
    return body->info;
}

void body_set_sprite_info(body_t *body, sprite_info_t info) {
    body->info = info;
}

body_shape_t body_get_body_shape(body_t *body) {
    return body->shape;
}

SDL_Rect body_get_shape(body_t *body) {
    return body->shape.shape;
}

rect_t body_get_hitbox(body_t *body) {
    return body->shape.hitbox;
}

rect_t body_get_collision_hitbox(body_t *body) {
    rect_t hitbox = body->shape.hitbox;
    SDL_Rect collision_box = body->shape.collision_shape;
    if(body->flipped) {
        hitbox.x += (body->shape.shape.w - (collision_box.x + collision_box.w)) * body->scale;
    } else {
        hitbox.x += collision_box.x * body->scale;
    }
    hitbox.w = collision_box.w * body->scale;
    hitbox.h = collision_box.h * body->scale;
    return hitbox;
}

bool body_get_flipped(body_t *body) {
    return body->flipped;
}

SDL_Texture *body_get_texture(body_t *body) {
    return body->texture;
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

double body_get_scale(body_t *body) {
    return body->scale;
}

/*
rgb_color_t body_get_color(body_t *body) {
    return body->color;
}
*/

char *body_get_type(body_t *body) {
    return body->type;
}

void body_set_centroid(body_t *body, vector_t x) {
    vector_t translation = vec_subtract(x, body->centroid);
    body->centroid = x;
    body->shape.hitbox.x += translation.x;
    body->shape.hitbox.y += translation.y;
}

void body_set_velocity(body_t *body, vector_t v) {
    body->velocity = v;
    if(v.x < 0) body->flipped = true;
    if(v.x > 0) body->flipped = false;
}

vector_t body_get_direction(body_t *body) {
    return body->direction;
}

void body_set_direction(body_t *body, vector_t dir) {
    body->direction = dir;
}

/*
void body_set_color(body_t *body, rgb_color_t color) {
    body->color = color;
}
*/

// NOTE: figure out if this would work to "rotate" the body, change the sprite that's being rendered
void body_set_rotation(body_t *body, SDL_Rect shape, double angle) {
    body->shape.shape = shape;
    body->rotation = angle;
    body->centroid.x = shape.x + shape.w / 2;
    body->centroid.x = shape.y - shape.h / 2;
}
/*
void body_rotate(body_t *body, double angle) {
    polygon_rotate(body->shape, angle, body->centroid);
    body->rotation += angle;
}
*/

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

vector_t body_calculate_velocity(body_t *body, double dt) {
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
    return final_velocity;
}

void body_tick(body_t *body, double dt) {
    vector_t final_velocity = body_calculate_velocity(body, dt);
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
