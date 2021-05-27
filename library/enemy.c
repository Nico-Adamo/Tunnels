#include "enemy.h"
#include "forces.h"
#include "projectile.h"
#include "sdl_wrapper.h"

const double BULLET_SPEED = 450;

const double enemy_cooldown = 5;
const double min_cooldown = 0.5;
const double max_cooldown = 2;

const char *BULLET_PATH = "assets/knight_f_idle_anim_f0.png";

bool has_line_of_sight(game_t *game, vector_t pos1, vector_t pos2, double dx) {
    scene_t *scene = game_get_current_scene(game);
    vector_t direction = vec_find_direction(pos2, pos1);
    vector_t cur_point = pos1;
    double distance = vec_distance(pos1, pos2);
    for(size_t i = 0; i < floor(distance)/dx; i++) {
        cur_point = vec_add(cur_point, vec_multiply(dx, direction));
        list_t *colliders = scene_get_collider_tiles(scene);
        for(size_t i = 0; i<list_size(colliders); i++) {
            tile_t *collider = list_get(colliders, i);
            if((cur_point.x >= collider->hitbox.x && cur_point.x <= collider->hitbox.x+collider->hitbox.w) &&
               (cur_point.y >= collider->hitbox.y && cur_point.y <= collider->hitbox.y+collider->hitbox.h)) {
                   return false;
            }
        }
    }
    return true;
}

void pathfind(game_t *game, body_t *enemy, body_t *player) {
    body_set_velocity(enemy, vec_multiply(body_get_stats_info(enemy).speed, vec_find_direction(body_get_centroid(player), body_get_centroid(enemy))));
}

void shooter_attack(game_t *game, body_t *enemy, body_t *player) {
    scene_t *scene = game_get_current_scene(game);
    stats_info_t enemy_info = body_get_stats_info(enemy);
    if (body_get_shoot_cooldown(enemy) == 0) {
        body_t *bullet = make_bullet(game, enemy, vec_find_direction(body_get_centroid(player), body_get_centroid(enemy)), body_get_stats_info(enemy).bullet_id, 400);
        scene_add_body(scene, bullet);
        create_tile_collision(scene, bullet);
        create_semi_destructive_collision(scene, game_get_player(game), bullet);
        body_set_shoot_cooldown(enemy, rand_from(enemy_info.cooldown-1, enemy_info.cooldown+1));
    }
}

void static_shooter_attack(game_t *game, body_t *enemy, body_t *player) {
    shooter_attack(game, enemy, player);
}

void radial_shooter_attack(game_t *game, body_t *enemy, body_t *player) {
    double distance = vec_distance(body_get_centroid(enemy), body_get_centroid(player));
    if(distance >= 100) {
        body_set_velocity(enemy, vec_multiply(body_get_stats_info(enemy).speed, vec_find_direction(body_get_centroid(player), body_get_centroid(enemy))));
    } else {
        body_set_velocity(enemy, VEC_ZERO);
    }
    shooter_attack(game, enemy, player);
}

void melee_shooter_attack(game_t *game, body_t *enemy, body_t *player) {
    double distance = vec_distance(body_get_centroid(enemy), body_get_centroid(player));
    if(distance >= 100) {
        shooter_attack(game, enemy, player);
    }
}

void handle_enemies(game_t *game, double dt) {
    scene_t *scene = game_get_current_scene(game);
    list_t *enemies = scene_get_enemies(scene);
    for(int i=0; i<list_size(enemies); i++) {
        body_t *enemy = list_get(enemies, i);
        stats_info_t enemy_info = body_get_stats_info(enemy);
        body_t *player = game_get_player(game);
        vector_t enemy_center = body_get_centroid(enemy);
        vector_t player_center = body_get_centroid(player);
        rect_t player_collision_hitbox = body_get_collision_hitbox(player);
        vector_t pch_center = (vector_t) {player_collision_hitbox.x + player_collision_hitbox.w/2, player_collision_hitbox.y + player_collision_hitbox.h/2};

        double distance = vec_distance(body_get_centroid(enemy), body_get_centroid(player));

        if(has_line_of_sight(game, enemy_center, pch_center, 16)) {
            if (!(enemy_info.atk_type == RADIAL_SHOOTER && vec_distance(body_get_centroid(enemy), body_get_centroid(player)) <= 100)) {
                pathfind(game, enemy, player);
            }
            if (sdl_is_onscreen(body_get_centroid(enemy).x, body_get_centroid(enemy).y)) {
                if(enemy_info.atk_type == MELEE) {
                    //do nothing
                } else if(enemy_info.atk_type == RADIAL_SHOOTER) {
                    radial_shooter_attack(game, enemy, player);
                } else if(enemy_info.atk_type == STATIC_SHOOTER) {
                    static_shooter_attack(game, enemy, player);
                } else if(enemy_info.atk_type == MELEE_SHOOTER) {
                    melee_shooter_attack(game, enemy, player);
                } else {
                    printf("Enemy with no valid atk type %d", enemy_info.atk_type);
                }
            }
        } else {
            body_set_velocity(enemy, VEC_ZERO);
        }

    }
}


// SPRITE INFO ARRAY
body_sprite_info_t ENEMY_SPRITE_INFOS[19] = {
    // TINY_ZOMBIE
    {.idle_sprite_id = 4,
     .walking_anim_id = 5,
     .hit_anim_id = -1,
     .invulnerable_anim_id = -1},
    // GOBLIN
    {.idle_sprite_id = 6,
     .walking_anim_id = 7,
     .hit_anim_id = -1,
     .invulnerable_anim_id = -1},
     // IMP
     {.idle_sprite_id = 8,
     .walking_anim_id = 9,
     .hit_anim_id = -1,
     .invulnerable_anim_id = -1},
     // SKELET
     {.idle_sprite_id = 10,
     .walking_anim_id = 11,
     .hit_anim_id = -1,
     .invulnerable_anim_id = -1},
     // MUDDY
     {.idle_sprite_id = 12,
     .walking_anim_id = -1,
     .hit_anim_id = -1,
     .invulnerable_anim_id = -1},
     // SWAMPY
     {.idle_sprite_id = 13,
     .walking_anim_id = -1,
     .hit_anim_id = -1,
     .invulnerable_anim_id = -1},
     // ZOMBIE
     {.idle_sprite_id = 14,
     .walking_anim_id = -1,
     .hit_anim_id = -1,
     .invulnerable_anim_id = -1},
     // ICE_ZOMBIE
     {.idle_sprite_id = 15,
     .walking_anim_id = -1,
     .hit_anim_id = -1,
     .invulnerable_anim_id = -1},
     // MASKED_ORC
     {.idle_sprite_id = 16,
     .walking_anim_id = 17,
     .hit_anim_id = -1,
     .invulnerable_anim_id = -1},
     // ORC_WARRIOR
     {.idle_sprite_id = 18,
     .walking_anim_id = 19,
     .hit_anim_id = -1,
     .invulnerable_anim_id = -1},
     // ORC_SHAMAN
     {.idle_sprite_id = 20,
     .walking_anim_id = 21,
     .hit_anim_id = -1,
     .invulnerable_anim_id = -1},
     // NECROMANCER
     {.idle_sprite_id = 22,
     .walking_anim_id = -1,
     .hit_anim_id = -1,
     .invulnerable_anim_id = -1},
     // WOGUL
     {.idle_sprite_id = 23,
     .walking_anim_id = 24,
     .hit_anim_id = -1,
     .invulnerable_anim_id = -1},
     // CHORT
     {.idle_sprite_id = 25,
     .walking_anim_id = 26,
     .hit_anim_id = -1,
     .invulnerable_anim_id = -1},
     // WIZARD
     {.idle_sprite_id = 27,
     .walking_anim_id = 28,
     .hit_anim_id = -1,
     .invulnerable_anim_id = -1},
     // NECROMANCER_WIZARD
     {.idle_sprite_id = 29,
     .walking_anim_id = 30,
     .hit_anim_id = -1,
     .invulnerable_anim_id = -1},
     // BIG_ZOMBIE
     {.idle_sprite_id = 31,
     .walking_anim_id = 32,
     .hit_anim_id = -1,
     .invulnerable_anim_id = -1},
     // OGRE
     {.idle_sprite_id = 33,
     .walking_anim_id = 34,
     .hit_anim_id = -1,
     .invulnerable_anim_id = -1},
     // BIG_DEMON
     {.idle_sprite_id = 35,
     .walking_anim_id = 36,
     .hit_anim_id = -1,
     .invulnerable_anim_id = -1}
};

    double experience;
    double health;
    double attack;
    double cooldown;
    double invulnerability_timer;
    int bullet_id;
    double speed;
    int level;
    enum attack_type atk_type;

stats_info_t ENEMY_STAT_INFO[19] = {
    // TINY_ZOMBIE
    {.health = 10,
     .attack = 2,
     .cooldown = 0,
     .bullet_id = 46,
     .experience = 5,
     .speed = 200,
     .atk_type = MELEE,
     .invulnerability_timer = 0
    },
    // GOBLIN
    {.health = 10,
     .attack = 2,
     .cooldown = 0,
     .bullet_id = 42,
     .experience = 5,
     .speed = 210,
     .atk_type = MELEE,
     .invulnerability_timer = 0
    },
     // IMP
    {.health = 10,
     .attack = 2,
     .cooldown = 0,
     .bullet_id = 37,
     .experience = 5,
     .speed = 215,
     .atk_type = MELEE,
     .invulnerability_timer = 0
    },
     // SKELET
    {.health = 15,
     .attack = 5,
     .cooldown = 2,
     .bullet_id = 44,
     .experience = 5,
     .speed = 130,
     .atk_type = RADIAL_SHOOTER,
     .invulnerability_timer = 0
    },
     // MUDDY
    {.health = 20,
     .attack = 5,
     .cooldown = 4,
     .bullet_id = 39,
     .experience = 10,
     .speed = 10,
     .atk_type = STATIC_SHOOTER,
     .invulnerability_timer = 0
    },
     // SWAMPY
    {.health = 20,
     .attack = 5,
     .cooldown = 4,
     .bullet_id = 45,
     .experience = 10,
     .speed = 15,
     .atk_type = STATIC_SHOOTER,
     .invulnerability_timer = 0
    },
     // ZOMBIE
    {.health = 15,
     .attack = 5,
     .cooldown = 2,
     .bullet_id = 46,
     .experience = 5,
     .speed = 90,
     .atk_type = MELEE_SHOOTER,
     .invulnerability_timer = 0
    },
     // ICE_ZOMBIE
    {.health = 15,
     .attack = 5,
     .cooldown = 2,
     .bullet_id = 38,
     .experience = 5,
     .speed = 110,
     .atk_type = MELEE_SHOOTER,
     .invulnerability_timer = 0
    },
     // MASKED_ORC
    {.health = 20,
     .attack = 5,
     .cooldown = 3,
     .bullet_id = 42,
     .experience = 10,
     .speed = 150,
     .atk_type = MELEE_SHOOTER,
     .invulnerability_timer = 0
    },
     // ORC_WARRIOR
    {.health = 20,
     .attack = 5,
     .cooldown = 3,
     .bullet_id = 42,
     .experience = 10,
     .speed = 150,
     .atk_type = RADIAL_SHOOTER,
     .invulnerability_timer = 0
    },
     // ORC_SHAMAN
    {.health = 20,
     .attack = 5,
     .cooldown = 3,
     .bullet_id = 42,
     .experience = 10,
     .speed = 10,
     .atk_type = STATIC_SHOOTER,
     .invulnerability_timer = 0
    },
     // NECROMANCER
    {.health = 30,
     .attack = 10,
     .cooldown = 5,
     .bullet_id = 42,
     .experience = 15,
     .speed = 10,
     .atk_type = STATIC_SHOOTER,
     .invulnerability_timer = 0
    },
     // WOGUL
    {.health = 30,
     .attack = 10,
     .cooldown = 4,
     .bullet_id = 37,
     .experience = 15,
     .speed = 150,
     .atk_type = RADIAL_SHOOTER,
     .invulnerability_timer = 0
    },
     // CHORT
    {.health = 30,
     .attack = 10,
     .cooldown = 4,
     .bullet_id = 37,
     .experience = 15,
     .speed = 150,
     .atk_type = MELEE_SHOOTER,
     .invulnerability_timer = 0
    },
     // WIZARD
    {.health = 30,
     .attack = 10,
     .cooldown = 5,
     .bullet_id = 47,
     .experience = 15,
     .speed = 15,
     .atk_type = STATIC_SHOOTER,
     .invulnerability_timer = 0
    },
     // NECROMANCER_WIZARD
    {.health = 40,
     .attack = 15,
     .cooldown = 5,
     .bullet_id = 41,
     .experience = 20,
     .speed = 150,
     .atk_type = STATIC_SHOOTER,
     .invulnerability_timer = 0
    },
     // BIG_ZOMBIE
    {.health = 40,
     .attack = 15,
     .cooldown = 5,
     .bullet_id = 46,
     .experience = 20,
     .speed = 100,
     .atk_type = STATIC_SHOOTER,
     .invulnerability_timer = 0
    },
     // OGRE
    {.health = 40,
     .attack = 15,
     .cooldown = 5,
     .bullet_id = 42,
     .experience = 20,
     .speed = 120,
     .atk_type = STATIC_SHOOTER,
     .invulnerability_timer = 0
    },
     // BIG_DEMON
    {.health = 60,
     .attack = 20,
     .cooldown = 5,
     .bullet_id = 37,
     .experience = 30,
     .speed = 130,
     .atk_type = STATIC_SHOOTER,
     .invulnerability_timer = 0
    }
};

body_sprite_info_t enemy_get_sprite_info(enum enemy_type type) {
    return ENEMY_SPRITE_INFOS[type];
}

stats_info_t enemy_get_stats(enum enemy_type type) {
    return ENEMY_STAT_INFO[type];
}
