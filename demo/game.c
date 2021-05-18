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

const char *SPRITE = "knight";
const char *SPRITE_PATH = "assets/knight_f_idle_anim_f0.png";

const double MAX_WIDTH = 1000;
const double MAX_HEIGHT = 500;

const vector_t PADDLE_VELOCITY = {
    .x = 300,
    .y = 0
};

double rand_from(double min, double max) {
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

void on_key(char key, key_event_type_t type, double held_time, scene_t *scene) {
    if (type == KEY_PRESSED) {
        switch (key) {
            case LEFT_ARROW:
                body_set_velocity(scene_get_body(scene, 0), vec_negate(PADDLE_VELOCITY)); //pass player first
                break;
            case RIGHT_ARROW:
                body_set_velocity(scene_get_body(scene, 0), PADDLE_VELOCITY);
                break;
        }
    }
    else if (type == KEY_RELEASED) {
        switch (key) {
            case LEFT_ARROW:
                body_set_velocity(scene_get_body(scene, 0), VEC_ZERO);
                break;
            case RIGHT_ARROW:
                body_set_velocity(scene_get_body(scene, 0), VEC_ZERO);
                break;
        }
    }
}

body_t *make_demo_sprite() {
    SDL_Texture *texture = sdl_load_texture(SPRITE_PATH);
    // First argument: Sprite size (x,y are always 0)
    // Second argument: Bottom left corner of sprite, and size (we should eventually change to scale factor rather than specifying explicit width and height)
    return body_init((SDL_Rect) {0, 0, 16, 32}, (rect_t) {100, 100, 160, 320}, texture, 10);
}

scene_t *scene_reset() {
    // Initialize Paddle and Ball
    body_t *sprite = make_demo_sprite();
    // Create Scene
    scene_t *scene = scene_init();
    scene_add_body(scene, sprite);

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
        int num_bricks = 0;
        double dt = time_since_last_tick();

        scene_tick(scene, dt);
        seconds += dt;
        sdl_render_scene(scene);
    }

    scene_free(scene);


}
