#include "sdl_wrapper.h"
#include "scene.h"
#include "color.h"
#include "list.h"
#include "vector.h"
#include "polygon.h"
#include "body.h"
#include <math.h>
#include <stdlib.h>
#include "forces.h"
#include <stdio.h>
#include <string.h>
#include "collision.h"

const char *SPRITE = "knight";
const char *SPRITE_PATH = "assets/knight_f_idle_anim_f0.png";


double BULLET_SPEED = 450;

const double MAX_WIDTH = 1000;
const double MAX_HEIGHT = 500;

const vector_t PADDLE_VELOCITY = {
    .x = 300,
    .y = 0
};

const vector_t PADDLE_UP_VELOCITY = {
    .x = 0,
    .y = 300
};

const double player_velocity = 300;

const double min_cooldown = 0.5;
const double max_cooldown = 2;

const double enemy_cooldown = 5;

double rand_from(double min, double max) {
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

body_t *make_demo_bullet(body_t *sprite, vector_t bullet_dir) {
    SDL_Texture *texture = sdl_load_texture(SPRITE_PATH);
    vector_t spawn_point = body_get_centroid(sprite);
    body_t *bullet;
    sprite_info_t info = {
        .experience = 0,
        .attack = body_get_sprite_info(sprite).attack,
        .health = 0,
        .cooldown = 0
    };
    if (body_get_type(sprite) == "PLAYER") {
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

void on_key(char key, key_event_type_t type, double held_time, scene_t *scene) {
    body_t *player = scene_get_body(scene, 0);
    vector_t velocity = body_get_velocity(player);
    vector_t bullet_dir = VEC_ZERO;
    if (type == KEY_PRESSED) {
        switch (key) {
            case 'a':
                velocity.x = -player_velocity;
                body_set_velocity(player, velocity); //pass player first
                body_set_direction(player, vec_unit(body_get_velocity(player)));
                break;
            case 'd':
                velocity.x = player_velocity;
                body_set_velocity(player, velocity);
                body_set_direction(player, vec_unit(body_get_velocity(player)));
                break;
            case 's':
                velocity.y = -player_velocity;
                body_set_velocity(player, velocity); //pass player first
                body_set_direction(player, vec_unit(body_get_velocity(player)));
                break;
            case 'w':
                velocity.y = player_velocity;
                body_set_velocity(player, velocity);
                body_set_direction(player, vec_unit(body_get_velocity(player)));
                break;
            case 'i':
                bullet_dir.y = 1;
                scene_add_body(scene, make_demo_bullet(player, bullet_dir));
                for (size_t i = 1; i < scene_bodies(scene); i++) {
                    if (strcmp(body_get_type(scene_get_body(scene, i)), "ENEMY") == 0) {
                        create_semi_destructive_collision(scene, scene_get_body(scene, i), scene_get_body(scene, scene_bodies(scene) - 1));
                    }
                }
                break;
            case 'j':
                bullet_dir.x = -1;
                scene_add_body(scene, make_demo_bullet(player, bullet_dir));
                for (size_t i = 1; i < scene_bodies(scene); i++) {
                    if (strcmp(body_get_type(scene_get_body(scene, i)), "ENEMY") == 0) {
                        create_semi_destructive_collision(scene, scene_get_body(scene, i), scene_get_body(scene, scene_bodies(scene) - 1));
                    }
                }
                break;
            case 'k':
                bullet_dir.y = -1;
                scene_add_body(scene, make_demo_bullet(player, bullet_dir));
                for (size_t i = 1; i < scene_bodies(scene); i++) {
                    if (strcmp(body_get_type(scene_get_body(scene, i)), "ENEMY") == 0) {
                        create_semi_destructive_collision(scene, scene_get_body(scene, i), scene_get_body(scene, scene_bodies(scene) - 1));
                    }
                }
                break;
            case 'l':
                bullet_dir.x = 1;
                scene_add_body(scene, make_demo_bullet(player, bullet_dir));
                for (size_t i = 1; i < scene_bodies(scene); i++) {
                    if (strcmp(body_get_type(scene_get_body(scene, i)), "ENEMY") == 0) {
                        create_semi_destructive_collision(scene, scene_get_body(scene, i), scene_get_body(scene, scene_bodies(scene) - 1));
                    }
                }
                break;
            /*case ' ':
                scene_add_body(scene, make_demo_bullet(player, bullet_dir));
                if (scene_bodies(scene) > 2) {
                    // much demo much wow, just creating a collision with what i know to be an enemy lol
                    create_destructive_collision(scene, scene_get_body(scene, 1), scene_get_body(scene, scene_bodies(scene) - 1));
                }
                break;
            */
        }
    }
    else if (type == KEY_RELEASED) {
        switch (key) {
            case 'a':
                body_set_direction(player, vec_unit(body_get_velocity(player)));
                velocity.x = 0;
                body_set_velocity(player, velocity);
                if (body_get_velocity(player).x != 0 || body_get_velocity(player).y != 0) {
                    body_set_direction(player, vec_unit(body_get_velocity(player)));
                }
                break;
            case 'd':
                body_set_direction(player, vec_unit(body_get_velocity(player)));
                velocity.x = 0;
                body_set_velocity(player, velocity);
                if (body_get_velocity(player).x != 0 || body_get_velocity(player).y != 0) {
                    body_set_direction(player, vec_unit(body_get_velocity(player)));
                }
                break;
            case 's':
                body_set_direction(player, vec_unit(body_get_velocity(player)));
                velocity.y = 0;
                body_set_velocity(player, velocity);
                if (body_get_velocity(player).x != 0 || body_get_velocity(player).y != 0) {
                    body_set_direction(player, vec_unit(body_get_velocity(player)));
                }
                break;
            case 'w':
                body_set_direction(player, vec_unit(body_get_velocity(player)));
                velocity.y = 0;
                body_set_velocity(player, velocity);
                if (body_get_velocity(player).x != 0 || body_get_velocity(player).y != 0) {
                    body_set_direction(player, vec_unit(body_get_velocity(player)));
                }
                break;
        }
    }
}

body_t *make_demo_sprite(double x, double y, char *type, sprite_info_t info) {
    SDL_Texture *texture = sdl_load_texture(SPRITE_PATH);
    // First argument: Sprite size (x,y are always 0)
    // Second argument: Bottom left corner of sprite, and size (we should eventually change to scale factor rather than specifying explicit width and height)
    return body_init_with_info((SDL_Rect) {0, 0, 16, 32}, (SDL_Rect) {16, 0, 50, 40}, (rect_t) {x, y, 80, 160}, texture, 100, 4, type, info);
}

scene_t *scene_reset() {
    sprite_info_t player_info = {
        .experience = 0,
        .attack = 5,
        .health = 50,
        .cooldown = 0
    };

    sprite_info_t enemy_info = {
        .experience = 0,
        .attack = 5,
        .health = 30,
        .cooldown = rand_from(min_cooldown, max_cooldown)
    };

    // Initialize Sprite/Player
    body_t *sprite = make_demo_sprite(100, 100, "PLAYER", player_info);

    // Initialize Potential Enemy
    body_t *temp_enemy = make_demo_sprite(400, 200, "ENEMY", enemy_info);


    // Create Scene
    scene_t *scene = scene_init();
    scene_add_body(scene, sprite);
    scene_add_body(scene, temp_enemy);

    return scene;
}

int main(int arg_c, char *arg_v[]) {
    sprite_info_t enemy_info = {
        .experience = 0,
        .attack = 5,
        .health = 30,
        .cooldown = rand_from(min_cooldown, max_cooldown)
    };

    vector_t bottom_left = {.x = 0, .y = 0};
    vector_t top_right = {.x = MAX_WIDTH, .y = MAX_HEIGHT};
    sdl_init(bottom_left, top_right);

    double seconds = 0;
    scene_t *scene = scene_reset();
    create_semi_destructive_collision(scene, scene_get_body(scene, 0), scene_get_body(scene, 1));

    sdl_on_key(on_key);
    while(!sdl_is_done(scene)) {
        double dt = time_since_last_tick();


        scene_tick(scene, dt);
        seconds += dt;
        for (size_t i = 0; i < scene_bodies(scene); i++) {
            if (strcmp(body_get_type(scene_get_body(scene, i)), "ENEMY") == 0) {
                body_t *enemy = scene_get_body(scene, i);
                sprite_info_t enemy_info = body_get_sprite_info(enemy);
                if (enemy_info.cooldown > 0) {
                    enemy_info.cooldown -= dt;
                    body_set_sprite_info(enemy, enemy_info);
                }
                else {
                    body_t *bullet = make_demo_bullet(enemy, find_bullet_direction(scene_get_body(scene, 0), enemy));
                    scene_add_body(scene, bullet);
                    create_semi_destructive_collision(scene, scene_get_body(scene, 0), bullet);
                    // create_newtonian_gravity(scene, 10000, scene_get_body(scene, 0), bullet);
                    enemy_info.cooldown = rand_from(min_cooldown, max_cooldown);
                    body_set_sprite_info(enemy, enemy_info);
                }

            }
        }
        int loc = 400;

        if (seconds >= enemy_cooldown) {
            loc += 100;
            body_t *new_temp_enemy = make_demo_sprite(loc, 200, "ENEMY", enemy_info);
            scene_add_body(scene, new_temp_enemy);
            create_semi_destructive_collision(scene, scene_get_body(scene, 0), new_temp_enemy);
            seconds = 0;
        }

        if (strcmp(body_get_type(scene_get_body(scene, 0)), "PLAYER") != 0) {
            scene = scene_reset();
        }



        sdl_render_scene(scene);
    }

    scene_free(scene);


}
