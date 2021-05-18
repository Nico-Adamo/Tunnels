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

double rand_from(double min, double max) {
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

body_t *make_demo_bullet(body_t *sprite) {
    SDL_Texture *texture = sdl_load_texture(SPRITE_PATH);
    vector_t spawn_point = body_get_centroid(sprite);
    body_t *bullet = body_init((SDL_Rect) {0, 0, 16, 32}, (rect_t) {spawn_point.x, spawn_point.y, 16, 32}, texture, 1);
    vector_t player_dir = body_get_direction(sprite);
    vector_t bullet_velocity = {
        .x = player_dir.x * BULLET_SPEED,
        .y = player_dir.y * BULLET_SPEED
    };
    body_set_velocity(bullet, bullet_velocity);
    return bullet;
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
            case ' ':
                scene_add_body(scene, make_demo_bullet(player));
                if (scene_bodies(scene) > 2) {
                    // much demo much wow, just creating a collision with what i know to be an enemy lol
                    create_destructive_collision(scene, scene_get_body(scene, 1), scene_get_body(scene, scene_bodies(scene) - 1));
                }
                break;
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

body_t *make_demo_sprite(double x, double y) {
    SDL_Texture *texture = sdl_load_texture(SPRITE_PATH);
    // First argument: Sprite size (x,y are always 0)
    // Second argument: Bottom left corner of sprite, and size (we should eventually change to scale factor rather than specifying explicit width and height)
    return body_init((SDL_Rect) {0, 0, 16, 32}, (rect_t) {x, y, 80, 160}, texture, 10);
}

scene_t *scene_reset() {
    // Initialize Sprite/Player
    body_t *sprite = make_demo_sprite(100, 100);

    // Initialize Potential Enemy
    body_t *temp_enemy = make_demo_sprite(400, 200);


    // Create Scene
    scene_t *scene = scene_init();
    scene_add_body(scene, sprite);
    scene_add_body(scene, temp_enemy);

    return scene;
}

int main(int arg_c, char *arg_v[]) {
    vector_t bottom_left = {.x = 0, .y = 0};
    vector_t top_right = {.x = MAX_WIDTH, .y = MAX_HEIGHT};
    sdl_init(bottom_left, top_right);

    double seconds = 0;
    scene_t *scene = scene_reset();

    sdl_on_key(on_key);
    while(!sdl_is_done(scene)) {
        double dt = time_since_last_tick();

        scene_tick(scene, dt);
        seconds += dt;
        sdl_render_scene(scene);
    }

    scene_free(scene);


}
