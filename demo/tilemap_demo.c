#include "sdl_wrapper.h"
#include "game.h"
#include "scene.h"
#include "color.h"
#include "list.h"
#include "vector.h"
#include "polygon.h"
#include "body.h"
#include "map.h"
#include <math.h>
#include <stdlib.h>
#include "forces.h"
#include <stdio.h>
#include <string.h>
#include "collision.h"

const char *SPRITE = "knight";
const char *SPRITE_PATH = "assets/knight_f_idle_anim_f0.png";


double BULLET_SPEED = 450;

const double MAX_WIDTH = 1024;
const double MAX_HEIGHT = 512;

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

void on_key(char key, key_event_type_t type, double held_time, scene_t *scene) {
    body_t *player = scene_get_body(scene, 0);
    vector_t velocity = body_get_velocity(player);
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
    return body_init_with_info((SDL_Rect) {0, 0, 16, 32}, (rect_t) {x, y, 64, 128}, texture, 100, type, info);
}

scene_t *scene_reset() {
    sprite_info_t player_info = {
        .experience = 0,
        .attack = 5,
        .health = 50,
        .cooldown = 0
    };

    // Initialize Sprite/Player
    body_t *sprite = make_demo_sprite(100, 100, "PLAYER", player_info);

    // Create Scene
    scene_t *scene = scene_init();
    scene_add_body(scene, sprite);

    return scene;
}

int main(int arg_c, char *arg_v[]) {

    vector_t bottom_left = {.x = 0, .y = 0};
    vector_t top_right = {.x = MAX_WIDTH, .y = MAX_HEIGHT};
    sdl_init(bottom_left, top_right);

    scene_t *scene = scene_reset();
    game_t *game = game_init(scene, 4);
    map_register_tiles(game);
    map_register_collider_tiles();
    map_load(game, "assets/levels/map.txt", 12, 7);
    double seconds = 0;
    create_tile_collision(scene, scene_get_body(scene, 0));

    sdl_on_key(on_key);
    while(!sdl_is_done(scene)) {
        double dt = time_since_last_tick();

        scene_tick(scene, dt);
        seconds += dt;
        sdl_set_camera(vec_subtract(body_get_centroid(scene_get_body(scene, 0)), (vector_t) {1024/2, 512/2}));

        sdl_render_scene(scene);
    }

    scene_free(scene);
    return 0;
}
