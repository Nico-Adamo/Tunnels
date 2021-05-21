#include "sdl_wrapper.h"
#include "game.h"
#include "enemy.h"
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
#include "keyhandler.h"
#include "user_interface.h"

const char *SPRITE = "knight";
const char *SPRITE_PATH = "assets/knight_f_idle_anim_f0.png";


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


body_t *make_demo_sprite(double x, double y, char *type, sprite_info_t info) {
    SDL_Texture *texture = sdl_load_texture(SPRITE_PATH);
    body_shape_t body_shape = {
        .shape = (SDL_Rect) {0, 0, 16, 32},
        .collision_shape = (SDL_Rect) {3, 0, 12, 6},
        .hitbox = (rect_t) {x, y, 64, 128}
    };
    // First argument: Sprite size (x,y are always 0)
    // Second argument: Bottom left corner of sprite, and size (we should eventually change to scale factor rather than specifying explicit width and height)
    return body_init_with_info(body_shape, texture, 100, 4, type, info);
}

scene_t *scene_reset() {

    sprite_info_t enemy_info = {
        .experience = 0,
        .attack = 5,
        .health = 30,
        .cooldown = rand_from(min_cooldown, max_cooldown)
    };

    // Initialize Sprite/Player
    scene_t *scene = scene_init();

    // Initialize Potential Enemy
    for(int i=0; i<1; i++) {
        body_t *temp_enemy = make_demo_sprite(400+100*i, 200, "ENEMY", enemy_info);
        scene_add_body(scene, temp_enemy);
    }

    // Create Scene

    return scene;
}

int main(int arg_c, char *arg_v[]) {
    vector_t bottom_left = {.x = 0, .y = 0};
    vector_t top_right = {.x = MAX_WIDTH, .y = MAX_HEIGHT};
    sdl_init(bottom_left, top_right);

    sprite_info_t player_info = {
        .experience = 0,
        .attack = 5,
        .health = 50,
        .cooldown = 0
    };

    body_t *player = make_demo_sprite(100, 100, "PLAYER", player_info);

    scene_t *scene = scene_reset();
    scene_add_body(scene, player);

    game_t *game = game_init(scene, player, 4);
    map_register_tiles(game);
    map_register_collider_tiles();
    map_load(game, "assets/levels/map_bigger.map", 20, 20);
    double seconds = 0;
    create_tile_collision(game_get_current_scene(game), game_get_player(game));

    sdl_on_key(on_key);
    while(!sdl_is_done(game)) {
        scene_t *scene = game_get_current_scene(game);
        double dt = time_since_last_tick();
        handle_enemies(game, dt);
        scene_tick(scene, dt);
        seconds += dt;
        sdl_set_camera(vec_subtract(body_get_centroid(game_get_player(game)), (vector_t) {1024 / 2, 512 / 2}));
        
        sdl_render_scene(scene);

        body_t *player = game_get_player(game);
        if (body_get_sprite_info(player).health <= 0) {
            scene_t *scene_new = scene_reset();
            player_info = body_get_sprite_info(player);
            player_info.health = 50;
            body_t *player_new = make_demo_sprite(100, 100, "PLAYER", player_info);
            game_set_player(game, player_new);
            game_set_current_scene(game, scene_new);
            scene_add_body(scene_new, player_new);
            scene_free(scene);
            map_load(game, "assets/levels/map_bigger.map", 20, 20);
            create_tile_collision(game_get_current_scene(game), game_get_player(game));

        }

    }
    game_free(game);
    return 0;
}
