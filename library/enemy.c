#include "enemy.h"
#include "forces.h"
#include "projectile.h"
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

void handle_enemies(game_t *game, double dt) {
    scene_t *scene = game_get_current_scene(game);
    list_t *enemies = scene_get_enemies(scene);
    for(int i=0; i<list_size(enemies); i++) {
        body_t *enemy = list_get(enemies, i);
        stats_info_t enemy_info = body_get_stats_info(enemy);
        if (enemy_info.cooldown > 0) {
            enemy_info.cooldown -= dt;
            body_set_stats_info(enemy, enemy_info);
        }
        else {
            body_t *bullet = make_bullet(game, enemy, find_direction(game_get_player(game), enemy), 2, 400);
            scene_add_body(scene, bullet);
            create_tile_collision(scene, bullet);
            create_semi_destructive_collision(scene, game_get_player(game), bullet);
                    //create_newtonian_gravity(scene, 10000, scene_get_body(scene, 0), bullet);
            enemy_info.cooldown = rand_from(0.2, 4);
            body_set_stats_info(enemy, enemy_info);
        }
    }
}
