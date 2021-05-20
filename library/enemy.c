#include "enemy.h"
#include "forces.h"
#include "sdl_wrapper.h"

const double BULLET_SPEED = 450;

const double enemy_cooldown = 5;
const double min_cooldown = 0.5;
const double max_cooldown = 2;

const char *BULLET_PATH = "assets/knight_f_idle_anim_f0.png";

double rand_from(double min, double max) {
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

body_t *make_demo_bullet(body_t *sprite, vector_t bullet_dir) {
    SDL_Texture *texture = sdl_load_texture(BULLET_PATH);
    vector_t spawn_point = body_get_centroid(sprite);
    body_t *bullet;
    sprite_info_t info = {
        .experience = 0,
        .attack = body_get_sprite_info(sprite).attack,
        .health = 0,
        .cooldown = 0
    };
    if (strcmp(body_get_type(sprite), "PLAYER")==0) {
        bullet = body_init_with_info((SDL_Rect) {0, 0, 16, 32}, (SDL_Rect) {3, 0, 10, 8}, (rect_t) {spawn_point.x, spawn_point.y, 16, 32}, texture, 0.1, 1, "PLAYER_BULLET", info);
    } else {
        bullet = body_init_with_info((SDL_Rect) {0, 0, 16, 32}, (SDL_Rect) {3, 0, 10, 8}, (rect_t) {spawn_point.x, spawn_point.y, 16, 32}, texture, 0.1, 1, "ENEMY_BULLET", info);
    }
    //vector_t player_dir = body_get_direction(sprite);
    vector_t bullet_velocity = {
        .x = bullet_dir.x * BULLET_SPEED,
        .y = bullet_dir.y * BULLET_SPEED
    };
    body_set_velocity(bullet, bullet_velocity);
    return bullet;
}

vector_t find_bullet_direction(body_t *player, body_t *enemy) {
    vector_t player_center = body_get_centroid(player);
    vector_t enemy_center = body_get_centroid(enemy);
    return vec_unit(vec_subtract(player_center, enemy_center));
}

void handle_enemies(game_t *game, double dt) {
    scene_t *scene = game_get_current_scene(game);
    list_t *enemies = scene_get_enemies(scene);
    for(int i=0; i<list_size(enemies); i++) {
        body_t *enemy = list_get(enemies, i);
        sprite_info_t enemy_info = body_get_sprite_info(enemy);
        if (enemy_info.cooldown > 0) {
            enemy_info.cooldown -= dt;
            body_set_sprite_info(enemy, enemy_info);
        }
        else {
            body_t *bullet = make_demo_bullet(enemy, find_bullet_direction(game_get_player(game), enemy));
            scene_add_body(scene, bullet);
            create_tile_collision(scene, bullet);
            create_semi_destructive_collision(scene, game_get_player(game), bullet);
                    //create_newtonian_gravity(scene, 10000, scene_get_body(scene, 0), bullet);
            enemy_info.cooldown = rand_from(0.2, 4);
            body_set_sprite_info(enemy, enemy_info);
        }
    }
}