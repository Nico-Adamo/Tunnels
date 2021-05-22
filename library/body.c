#include "body.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

// TO DO: Once sprite rendering is done, include it in typedef body_t
typedef struct body {
    sprite_t *sprite;
    vector_t bottom_left;
    double mass;
    double scale;
    vector_t centroid;
    vector_t velocity;
    vector_t net_force;
    vector_t net_impulse;
    bool removed;
    bool flipped;
    char *type;
    stats_info_t info;
    free_func_t info_freer;
    vector_t direction;
    double animation_timer;
} body_t;

body_t *body_init(sprite_t *sprite, vector_t bottom_left, double mass, double scale) {
    return body_init_with_info(sprite, bottom_left, mass, scale, NULL, (stats_info_t) {0.0, 0.0, 0.0, 0.0});
}

body_t *body_init_with_info(sprite_t *sprite, vector_t bottom_left, double mass, double scale, char *type, stats_info_t info) {
    body_t *body = malloc(sizeof(body_t));
    assert(body != NULL);

    body->sprite = sprite;
    body->bottom_left = bottom_left;
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
    body->animation_timer = 0;


    body->centroid.x = body->bottom_left.x + body->scale * (sprite_get_hitbox_shape(sprite).x + sprite_get_hitbox_shape(sprite).w / 2);
    body->centroid.y = body->bottom_left.y + body->scale * (sprite_get_hitbox_shape(sprite).y + sprite_get_hitbox_shape(sprite).h / 2);
    body->velocity = VEC_ZERO;

    return body;
}

// Texture??
void body_free(body_t *body) {
    free(body);
}

stats_info_t body_get_stats_info(body_t *body) {
    return body->info;
}

void body_set_stats_info(body_t *body, stats_info_t info) {
    body->info = info;
}

SDL_Rect body_get_hitbox_shape(body_t *body) {
    return sprite_get_hitbox_shape(body->sprite);
}

SDL_Rect body_get_draw_shape(body_t *body) {
    return sprite_get_shape(body->sprite);
}

SDL_Rect body_get_collision_shape(body_t *body) {
    return sprite_get_collision_shape(body->sprite);
}

rect_t body_get_draw_hitbox(body_t *body) {
    return (rect_t) {body->bottom_left.x, body->bottom_left.y, sprite_get_shape(body->sprite).w*body->scale, sprite_get_shape(body->sprite).h*body->scale};
}

rect_t body_get_hitbox(body_t *body) {
    rect_t hitbox = body_get_draw_hitbox(body);
    SDL_Rect collision_box = sprite_get_hitbox_shape(body->sprite);
    if(body->flipped) {
        hitbox.x += (sprite_get_shape(body->sprite).w - (collision_box.x + collision_box.w)) * body->scale;
    } else {
        hitbox.x += collision_box.x * body->scale;
    }
    hitbox.w = collision_box.w * body->scale;
    hitbox.h = collision_box.h * body->scale;
    return hitbox;
}

rect_t body_get_collision_hitbox(body_t *body) {
    rect_t hitbox = body_get_draw_hitbox(body);
    SDL_Rect collision_box = sprite_get_collision_shape(body->sprite);
    if(body->flipped) {
        hitbox.x += (sprite_get_shape(body->sprite).w - (collision_box.x + collision_box.w)) * body->scale;
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
    return sprite_get_texture(body->sprite);
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
    body->bottom_left.x += translation.x;
    body->bottom_left.y += translation.y;
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

    if (sprite_is_animated(body->sprite)) {
        body->animation_timer += dt;
        if (body->animation_timer >= sprite_get_animation_speed(body->sprite)) {
            sprite_set_current_frame(body->sprite, (sprite_get_current_frame(body->sprite) + 1) % sprite_get_animation_frames(body->sprite));
            body->animation_timer = 0;
        }
    }
    
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
