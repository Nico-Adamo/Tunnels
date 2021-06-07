#include "projectile.h"

const double BULLET_MASS = 0.1;
const double BULLET_SCALE = 4;
const double BULLET_SHIFT_SCALE = 0.5;

body_t *make_bullet(game_t *game, body_t *body, vector_t bullet_dir, int sprite_id, double bullet_speed) {
    sprite_t *sprite = game_get_sprite(game, sprite_id);
    body_sprite_info_t bullet_info = {
        .idle_sprite_id = sprite_id,
        .walking_anim_id = -1,
        .hit_anim_id = -1,
        .invulnerable_anim_id = -1
    };
    body_t *bullet;
    stats_info_t info = {
        .experience = 0,
        .attack = body_get_stats_info(body).attack,
        .health = 0,
        .cooldown = 0
    };

    if (body_get_type(body) == PLAYER) {
        bullet = body_init_with_info(bullet_info, sprite, body_get_centroid(body), BULLET_MASS, BULLET_SCALE, PLAYER_BULLET, info);
    } else {
        bullet = body_init_with_info(bullet_info, sprite, body_get_centroid(body), BULLET_MASS, BULLET_SCALE, ENEMY_BULLET, info);
    }

    vector_t bullet_velocity = {
        .x = bullet_dir.x * bullet_speed,
        .y = bullet_dir.y * bullet_speed
    };

    vector_t bullet_shift = {
        .x = (1 - abs(bullet_dir.x)) * BULLET_SHIFT_SCALE * body_get_velocity(body).x,
        .y = (1 - abs(bullet_dir.y)) * BULLET_SHIFT_SCALE * body_get_velocity(body).y
    };

    bullet_velocity = vec_add(bullet_velocity, bullet_shift);

    body_set_velocity(bullet, bullet_velocity);
    return bullet;

}

body_t *make_bullet_pos(game_t *game, vector_t spawn_point, vector_t bullet_dir, int sprite_id, int atk, double bullet_speed) {
    sprite_t *sprite = game_get_sprite(game, sprite_id);
    body_sprite_info_t bullet_info = {
        .idle_sprite_id = sprite_id,
        .walking_anim_id = -1,
        .hit_anim_id = -1,
        .invulnerable_anim_id = -1
    };
    body_t *bullet;
    stats_info_t info = {
        .experience = 0,
        .attack = atk,
        .health = 0,
        .cooldown = 0
    };

    bullet = body_init_with_info(bullet_info, sprite, spawn_point, BULLET_MASS, BULLET_SCALE, ENEMY_BULLET, info);

    vector_t bullet_velocity = {
        .x = bullet_dir.x * bullet_speed,
        .y = bullet_dir.y * bullet_speed
    };

    body_set_velocity(bullet, bullet_velocity);
    return bullet;

}
