#include "enemy.h"
#include "forces.h"
#include "projectile.h"
#include "sdl_wrapper.h"

const double BULLET_SPEED = 450;

const double enemy_cooldown = 5;
const double min_cooldown = 0.5;
const double max_cooldown = 2;

const double NECROMANCER_WALK_RADIUS = 75;
const double NECROMANCER_SPAWN_THICKNESS = 100;

const char *BULLET_PATH = "assets/knight_f_idle_anim_f0.png";

double BOSS_ATTACK_TIMER = 0;

body_t *make_enemy(game_t *game, double x, double y, enum enemy_type type) {
    vector_t bottom_left = {x, y};
    stats_info_t info = enemy_get_stats(type);
    body_sprite_info_t enemy_sprite_info = enemy_get_sprite_info(type);
    body_t *enemy;
    if(type >= NECROMANCER_WIZARD) { // Boss IDs
        enum body_type id;
        if(type == NECROMANCER_WIZARD) id = BOSS_NECROMANCER_WIZARD;
        if(type == BIG_ZOMBIE) id = BOSS_BIG_ZOMBIE;
        if(type == OGRE) id = BOSS_OGRE;
        if(type == BIG_DEMON) id = BOSS_BIG_DEMON;
        enemy = body_init_with_info(enemy_sprite_info, game_get_sprite(game, enemy_sprite_info.idle_sprite_id), bottom_left, 100, 4, id, info);
    } else {
        enemy = body_init_with_info(enemy_sprite_info, game_get_sprite(game, enemy_sprite_info.idle_sprite_id), bottom_left, 100, 4, ENEMY, info);
    }
    return enemy;
}

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

void pathfind_tile(game_t *game, body_t *body, vector_t pos) {
    body_set_velocity(body, vec_multiply(body_get_stats_info(body).speed, vec_find_direction(pos, body_get_centroid(body))));
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

void handle_non_boss_enemy(game_t *game, body_t *enemy) {
    stats_info_t enemy_info = body_get_stats_info(enemy);
    body_t *player = game_get_player(game);
    vector_t enemy_center = body_get_centroid(enemy);
    vector_t player_center = body_get_centroid(player);
    rect_t player_collision_hitbox = body_get_collision_hitbox(player);
    vector_t pch_center = (vector_t) {player_collision_hitbox.x + player_collision_hitbox.w/2, player_collision_hitbox.y + player_collision_hitbox.h/2};

    double distance = vec_distance(body_get_centroid(enemy), body_get_centroid(player));

    if(has_line_of_sight(game, enemy_center, pch_center, 16) && distance <= 2000) {
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

void necromancer_wizard_pathfind(game_t *game, body_t *enemy) {
    vector_t centroid = body_get_centroid(enemy);
    vector_t room_center = (vector_t) {830, 845};
    vector_t pathfind_dir = VEC_ZERO;
    double dist_from_center = vec_distance(centroid, room_center);

    if(body_get_velocity(enemy).x == 0 && body_get_velocity(enemy).y == 0) {
        if (dist_from_center >= NECROMANCER_WALK_RADIUS) {
            pathfind_tile(game, enemy, room_center);
        } else {
            double angle = rand_from (0, 2 * M_PI);
            pathfind_dir = (vector_t) {cos(angle), sin(angle)};
            printf("%f; %f, %f\n", angle, pathfind_dir.x, pathfind_dir.y);

            pathfind_tile(game, enemy, vec_add(room_center, vec_multiply(NECROMANCER_WALK_RADIUS, pathfind_dir)));
        }
    } else if (dist_from_center >= NECROMANCER_WALK_RADIUS || dist_from_center <= 15) { // Stopping her near enough to the center
        body_set_velocity(enemy, VEC_ZERO);
    }
}

void handle_necromancer_wizard(game_t *game, body_t *enemy) {

    double max_spawn_radius = 350;
    BOSS_ATTACK_TIMER -= 0.5;

    if (BOSS_ATTACK_TIMER <= 0) {
        // SPAWNING ATTACK

        for (size_t i = 0; i < (rand() % 5) + 2; i++) {
            double angle = rand_from(0, 2*M_PI);
            double radius = rand_from(max_spawn_radius - NECROMANCER_SPAWN_THICKNESS, max_spawn_radius);
            double x = body_get_centroid(enemy).x + (radius * cos(angle));
            double y = body_get_centroid(enemy).y + (radius * sin(angle));
            enum enemy_type ID;
            int ID_gen = rand() % 5;
            if (ID_gen == 0) ID = TINY_ZOMBIE;
            if (ID_gen == 1) ID = SKELET;
            if (ID_gen == 2) ID = ZOMBIE;
            if (ID_gen == 3) ID = ICE_ZOMBIE;
            if (ID_gen == 4) ID = MUDDY;
            body_t *new_enemy = make_enemy(game, x, y, ID);
            scene_add_body(game_get_current_scene(game), new_enemy);
            create_enemy_collision(game_get_current_scene(game), new_enemy, game_get_player(game));
        }
        BOSS_ATTACK_TIMER = rand_from(5, 6.5);
    } else {
        size_t shooting_attack = (rand() % 4); //50% chance to shoot every 0.5 seconds

        if (shooting_attack == 0) {
            // CIRCULAR BULLET ATTACK
            scene_t *scene = game_get_current_scene(game);
            size_t num_bullets = 4 * ((rand() % 3) + 1);
            for (size_t i = 0; i < num_bullets; i++) {
                double angle = i * 2 * M_PI / num_bullets;
                body_t *bullet = make_bullet(game, enemy, (vector_t) {cos(angle), sin(angle)}, body_get_stats_info(enemy).bullet_id, 400);

                scene_add_body(scene, bullet);
                create_tile_collision(scene, bullet);
                create_semi_destructive_collision(scene, game_get_player(game), bullet);
            }
        } else if (shooting_attack == 1) {

            // TARGETED BULLET ATTACK
            body_t *player = game_get_player(game);
            if(has_line_of_sight(game, body_get_centroid(enemy), body_get_centroid(player), 16)) {
                scene_t *scene = game_get_current_scene(game);
                vector_t player_direction = vec_find_direction(body_get_centroid(player), body_get_centroid(enemy));
                double player_angle = atan2(player_direction.y, player_direction.x);

                size_t num_bullets = 2 * (rand() % 3) + 1;
                double angle = player_angle - ((M_PI / 12) * (num_bullets - 1) / 2);
                for (size_t i = 0; i < num_bullets; i++) {
                    body_t *bullet = make_bullet(game, enemy, (vector_t) {cos(angle), sin(angle)}, body_get_stats_info(enemy).bullet_id, 400);

                    scene_add_body(scene, bullet);
                    create_tile_collision(scene, bullet);
                    create_semi_destructive_collision(scene, game_get_player(game), bullet);

                    angle += M_PI / 12;
                }
            }
        } else {
            necromancer_wizard_pathfind(game, enemy);
        }

    }

}

void handle_big_zombie(game_t *game, body_t *enemy) {
    // follow the player
    // shoots at player
    // rarely, raise the dead (zombie and tiny zombie only)
    // regaining health when hitting the player
}

void handle_ogre(game_t *game, body_t *enemy) {
    // mainly melee
    // charge attack
    // spawn army of orc dudes
    // rare shooting attack
}

void demon_pathfind(game_t *game, body_t *demon) {
    vector_t centroid = body_get_centroid(demon);
    vector_t pathfind_dir = VEC_ZERO;

    if(centroid.x < 800) {
        pathfind_dir = vec_add(pathfind_dir, (vector_t) {1, 0});
    } else if(centroid.x > 1000) {
        pathfind_dir = vec_add(pathfind_dir, (vector_t) {-1, 0});
    }
    if(centroid.y < 800) {
        pathfind_dir = vec_add(pathfind_dir, (vector_t) {0, 1});
    } else if(centroid.y > 1000) {
        pathfind_dir = vec_add(pathfind_dir, (vector_t) {0, -1});
    }
    if(pathfind_dir.x != 0 || pathfind_dir.y != 0) {
        if(pathfind_dir.x == 0) pathfind_dir.x = rand_from(-1,1);
        if(pathfind_dir.y == 0) pathfind_dir.y = rand_from(-1,1);
        pathfind_tile(game, demon, vec_add(centroid, pathfind_dir));
    }
    if(body_get_velocity(demon).x == 0 && body_get_velocity(demon).y == 0) {
        pathfind_tile(game, demon, vec_add(centroid, (vector_t) {1, -1}));
    }
}

void handle_big_demon(game_t *game, body_t *enemy) {

    scene_t *scene = game_get_current_scene(game);
    int bullet_num = 20;
    double max_spawn_radius = 250;
    BOSS_ATTACK_TIMER -= 0.5;

    demon_pathfind(game, enemy);

    if (BOSS_ATTACK_TIMER <= 0) {
        int atk_type = rand() % 4;
        if(atk_type == 0) {
            int bullet_speed = rand() % 300 + 300;
            for (size_t i = 0; i < bullet_num; i++) { // Circle
                double angle = 2*M_PI / bullet_num * i;
                body_t *bullet = make_bullet(game, enemy, (vector_t) {cos(angle), sin(angle)}, body_get_stats_info(enemy).bullet_id, bullet_speed);
                scene_add_body(scene, bullet);
                create_tile_collision(scene, bullet);
                create_semi_destructive_collision(scene, game_get_player(game), bullet);
            }
            BOSS_ATTACK_TIMER = rand_from(0, 1.5);
        } else if(atk_type == 1) { // Spiral
            double angle_offset = rand_from(0, 2 * M_PI);
            for (size_t i = 0; i < bullet_num; i++) {
                double angle = 2*M_PI / bullet_num * i + angle_offset;
                body_t *bullet = make_bullet(game, enemy, (vector_t) {cos(angle), sin(angle)}, body_get_stats_info(enemy).bullet_id, 200.0 / bullet_num * i + 200);
                scene_add_body(scene, bullet);
                create_tile_collision(scene, bullet);
                create_semi_destructive_collision(scene, game_get_player(game), bullet);
            }
            BOSS_ATTACK_TIMER = rand_from(0, 1.5);
        } else if(atk_type == 2) { // Bullet wall
            body_t *player = game_get_player(game);
            vector_t direction = vec_find_direction(body_get_centroid(player), body_get_centroid(enemy));
            vector_t normal_dir = (vector_t) {direction.y, -direction.x};
            int num_bullets = rand() % bullet_num/2 + bullet_num/2;
            for (size_t i = 0; i < num_bullets; i++) {
                vector_t offset = vec_multiply(20 * (bullet_num / 2.0 - i), normal_dir);
                vector_t spawn_pos = vec_add(body_get_centroid(enemy), offset);
                body_t *bullet = make_bullet_pos(game, spawn_pos, direction, body_get_stats_info(enemy).bullet_id, body_get_stats_info(enemy).attack, 400);
                scene_add_body(scene, bullet);
                create_tile_collision(scene, bullet);
                create_semi_destructive_collision(scene, game_get_player(game), bullet);
            }
            BOSS_ATTACK_TIMER = rand_from(0, 1.5);
        } else if(atk_type == 3) { // Spawn demon
            if(rand() % 4 == 0) {
                double angle = rand_from(0, 2*M_PI);
                double radius = rand_from(max_spawn_radius - NECROMANCER_SPAWN_THICKNESS, max_spawn_radius);
                double x = body_get_centroid(enemy).x + (radius * cos(angle));
                double y = body_get_centroid(enemy).y + (radius * sin(angle));
                enum enemy_type ID;
                int ID_gen = rand() % 3;
                if (ID_gen == 0) ID = IMP;
                if (ID_gen == 1) ID = WOGUL;
                if (ID_gen == 2) ID = CHORT;
                body_t *new_enemy = make_enemy(game, x, y, ID);
                scene_add_body(game_get_current_scene(game), new_enemy);
                create_enemy_collision(game_get_current_scene(game), new_enemy, game_get_player(game));
                BOSS_ATTACK_TIMER = rand_from(1, 2);
            }
        }

    }

    // spawn demons
}


void handle_enemies(game_t *game, double dt) {
    scene_t *scene = game_get_current_scene(game);
    list_t *enemies = scene_get_enemies(scene);
    for(int i = 0; i < list_size(enemies); i++) {
        body_t *enemy = list_get(enemies, i);
        if (body_get_type(enemy) == ENEMY) {
            handle_non_boss_enemy(game, enemy);
        } else if (body_get_type(enemy) == BOSS_NECROMANCER_WIZARD) {
            handle_necromancer_wizard(game, enemy);
        } else if (body_get_type(enemy) == BOSS_BIG_ZOMBIE) {

        } else if (body_get_type(enemy) == BOSS_OGRE) {

        } else if (body_get_type(enemy) == BOSS_BIG_DEMON) {
            handle_big_demon(game, enemy);
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
    {.health = 140,
     .attack = 15,
     .cooldown = 5,
     .bullet_id = 41,
     .experience = 20,
     .speed = 60,
     .atk_type = STATIC_SHOOTER,
     .invulnerability_timer = 0
    },
     // BIG_ZOMBIE
    {.health = 140,
     .attack = 15,
     .cooldown = 5,
     .bullet_id = 46,
     .experience = 20,
     .speed = 100,
     .atk_type = STATIC_SHOOTER,
     .invulnerability_timer = 0
    },
     // OGRE
    {.health = 140,
     .attack = 15,
     .cooldown = 5,
     .bullet_id = 42,
     .experience = 20,
     .speed = 120,
     .atk_type = STATIC_SHOOTER,
     .invulnerability_timer = 0
    },
     // BIG_DEMON
    {.health = 200,
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
