#include "body.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

double rand_from(double min, double max) {
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

typedef struct body {
    body_sprite_info_t sprite_ids;
    int cur_sprite_id;
    sprite_t *sprite;
    size_t cur_frame;
    vector_t bottom_left;
    double mass;
    double scale;
    vector_t centroid;
    vector_t velocity;
    vector_t net_force;
    vector_t net_impulse;
    bool removed;
    bool flipped;
    enum body_type type;
    stats_info_t info;
    free_func_t info_freer;
    vector_t direction;
    double animation_timer;
    double hit_timer;
    double shoot_cooldown;
    double invulnerability_timer;
} body_t;


body_t *body_init_with_info(body_sprite_info_t sprite_ids, sprite_t *sprite, vector_t bottom_left, double mass, double scale, enum body_type type, stats_info_t info) {
    body_t *body = malloc(sizeof(body_t));
    assert(body != NULL);

    body->sprite_ids = sprite_ids;
    body->cur_sprite_id = sprite_ids.idle_sprite_id;
    body->sprite = sprite;
    body->cur_frame = 0;
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
    body->hit_timer = 0;
    body->invulnerability_timer = 0;
    body->shoot_cooldown = rand_from(0.1, info.cooldown);


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

body_sprite_info_t body_get_sprite_ids(body_t *body) {
    return body->sprite_ids;
}

int body_get_cur_sprite_id(body_t *body) {
    return body->cur_sprite_id;
}

void body_set_sprite_ids(body_t *body, body_sprite_info_t sprite_ids) {
    body->sprite_ids = sprite_ids;
}

SDL_Rect body_get_hitbox_shape(body_t *body) {
    return sprite_get_hitbox_shape(body->sprite);
}

SDL_Rect body_get_draw_shape(body_t *body) {
    return sprite_get_shape(body->sprite, body->cur_frame);
}

SDL_Rect body_get_collision_shape(body_t *body) {
    return sprite_get_collision_shape(body->sprite);
}

rect_t body_get_draw_hitbox(body_t *body) {
    return (rect_t) {body->bottom_left.x, body->bottom_left.y, sprite_get_shape(body->sprite, body->cur_frame).w*body->scale, sprite_get_shape(body->sprite, body->cur_frame).h*body->scale};
}

rect_t body_get_hitbox(body_t *body) {
    rect_t hitbox = body_get_draw_hitbox(body);
    SDL_Rect collision_box = sprite_get_hitbox_shape(body->sprite);
    if(body->flipped) {
        hitbox.x += (sprite_get_shape(body->sprite, body->cur_frame).w - (collision_box.x + collision_box.w)) * body->scale;
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
        hitbox.x += (sprite_get_shape(body->sprite, body->cur_frame).w - (collision_box.x + collision_box.w)) * body->scale;
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

sprite_t *body_get_sprite(body_t *body) {
    return body->sprite;
}

void body_set_sprite(body_t *body, sprite_t *sprite) {
    body->sprite = sprite;
}

double body_get_invulnerability_timer(body_t *body) {
    return body->invulnerability_timer;
}

void body_set_invulnerability_timer(body_t *body, double invulnerability_timer) {
    body->invulnerability_timer = invulnerability_timer;
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

enum body_type body_get_type(body_t *body) {
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

    if(body->shoot_cooldown > 0) {
        body->shoot_cooldown -= dt;
        if(body->shoot_cooldown <= 0) body->shoot_cooldown = 0;
    }
    if(body->hit_timer > 0) {
        body->hit_timer -= dt;
        if(body->hit_timer <= 0) body->hit_timer = 0;
    }
    if (sprite_is_animated(body->sprite)) {
        if(body->cur_frame >= sprite_get_animation_frames(body->sprite)) {
            body->cur_frame = 0;
        }
        body->animation_timer += dt;
        if (body->animation_timer >= sprite_get_animation_speed(body->sprite)) {
            body->cur_frame = (body->cur_frame + 1) % sprite_get_animation_frames(body->sprite);
            body->animation_timer = 0;
        }
        if(body->invulnerability_timer > 0 && body->sprite_ids.invulnerable_anim_id != -1) {
            body->invulnerability_timer -= dt;
            if(body->cur_sprite_id != body->sprite_ids.invulnerable_anim_id) {
                body->cur_sprite_id = body->sprite_ids.invulnerable_anim_id;
                body->cur_frame = 0;
            }
        } else if((body->velocity.x != 0 || body->velocity.y != 0) && (body->sprite_ids.walking_anim_id != -1)) {
            body->cur_sprite_id = body->sprite_ids.walking_anim_id;
        } else {
            body->cur_sprite_id = body->sprite_ids.idle_sprite_id;
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

void body_set_shoot_cooldown(body_t *body, double cooldown) {
    body->shoot_cooldown = cooldown;
}

double body_get_shoot_cooldown(body_t *body) {
    return body->shoot_cooldown;
}

void body_set_hit_timer(body_t *body, double hit_timer) {
    body->hit_timer = hit_timer;
}

double body_get_hit_timer(body_t *body) {
    return body->hit_timer;
}
