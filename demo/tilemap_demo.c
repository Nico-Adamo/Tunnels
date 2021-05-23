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
#include "user_interface.h";
#include "sprite.h"
#include "sdl_wrapper.h"

const char *SPRITE = "knight";
// const char *SPRITE_PATH = "assets/knight_f_idle_anim_f0.png";
const char *FULL_HEART = "assets/ui/ui_heart_full.png";
const char *HALF_HEART = "assets/ui/ui_heart_half.png";
const char *EMPTY_HEART = "assets/ui/ui_heart_empty.png";
const char *START = "assets/ui/game_start_screen.png";


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

const double HALF_HEART_HEALTH = 5;
const double PLAYER_HEALTH = 100;
const double HEART_PADDING = 4;


body_t *make_demo_sprite(game_t *game, double x, double y, char *type, stats_info_t info) {
    vector_t bottom_left = {x, y};
    body_sprite_info_t player_sprite_info = {
        .idle_sprite_id = 0,
        .walking_anim_id = 4,
        .hit_anim_id = -1,
        .invulnerable_anim_id = 5
    };
    // First argument: Sprite size (x,y are always 0)
    // Second argument: Bottom left corner of sprite, and size (we should eventually change to scale factor rather than specifying explicit width and height)
    return body_init_with_info(player_sprite_info, game_get_sprite(game, player_sprite_info.idle_sprite_id), bottom_left, 100, 4, type, info);
}

body_t *make_enemy_sprite(game_t *game, double x, double y, char *type, stats_info_t info) {
    vector_t bottom_left = {x, y};
    body_sprite_info_t enemy_sprite_info = {
        .idle_sprite_id = 3,
        .walking_anim_id = -1,
        .hit_anim_id = -1,
        .invulnerable_anim_id = -1
    };
    // First argument: Sprite size (x,y are always 0)
    // Second argument: Bottom left corner of sprite, and size (we should eventually change to scale factor rather than specifying explicit width and height)
    return body_init_with_info(enemy_sprite_info, game_get_sprite(game, enemy_sprite_info.idle_sprite_id), bottom_left, 100, 4, type, info);
}
UI_t *make_demo_heart(double x, double y, char *texture_name, char *type) {
    SDL_Texture *texture = sdl_load_texture(texture_name);
    SDL_Rect shape = (SDL_Rect) {0, 0, 16, 16};
    rect_t hitbox = (rect_t) {x, y, 32, 32};
    return UI_init(shape, hitbox, texture, type, 2);
}

list_t *get_player_hearts(scene_t *scene) {
    list_t *UIs = scene_get_UI_components(scene);
    list_t *hearts = list_init(list_size(UIs), NULL);
    for (size_t i = 0; i < list_size(UIs); i++) {
        if (strcmp(UI_get_type(list_get(UIs, i)), "PLAYER_HEART") == 0) {
            list_add(hearts, list_get(UIs, i));
        }
    }
    return hearts;
}

scene_t *scene_reset(game_t *game) {

    stats_info_t enemy_info = {
        .experience = 0,
        .attack = 5,
        .health = 30,
        .cooldown = rand_from(.5, 2)
    };

    // Initialize Sprite/Player
    scene_t *scene = scene_init();

    // Initialize Potential Enemy
    for(int i=0; i<3; i++) {
        body_t *temp_enemy = make_enemy_sprite(game, 400+100*i, 200 + rand_from(-50,50), "ENEMY", enemy_info);
        scene_add_body(scene, temp_enemy);
    }

    // Initialize Demo Heart
    int heart_num = PLAYER_HEALTH / (HALF_HEART_HEALTH * 2);
    for (int i = 0; i < heart_num; i++) {
        UI_t *heart = make_demo_heart(HEART_PADDING + 32 * i, MAX_HEIGHT - 32 - HEART_PADDING, FULL_HEART, "PLAYER_HEART");
        scene_add_UI_component(scene, heart);
    }

    //Initialize Game Screen
    SDL_Texture *start = sdl_load_texture(START);
    SDL_Rect shape = (SDL_Rect) {0, 0, 1024, 512};
    rect_t hitbox = (rect_t) {0, 0, 1024, 512};
    UI_t *start_screen = UI_init(shape, hitbox, start, "START", 2);
    scene_add_UI_component(scene, start_screen);

    // Create Scene

    return scene;
}

int main(int arg_c, char *arg_v[]) {
    vector_t bottom_left = {.x = 0, .y = 0};
    vector_t top_right = {.x = MAX_WIDTH, .y = MAX_HEIGHT};
    sdl_init(bottom_left, top_right);

    stats_info_t player_info = {
        .experience = 0,
        .attack = 5,
        .health = PLAYER_HEALTH,
        .cooldown = 0
    };

    game_t *game = game_init(4);

    map_register_tiles(game);
    map_register_collider_tiles();

    game_register_sprites(game);

    body_t *player = make_demo_sprite(game, 100, 100, "PLAYER", player_info);
    scene_t *scene = scene_reset(game);

    scene_add_body(scene, player);

    game_set_player(game, player);
    game_set_current_scene(game, scene);
    map_load(game, "assets/levels/map_bigger.map", 20, 20);



    double seconds = 0;
    create_tile_collision(game_get_current_scene(game), game_get_player(game));
    SDL_Texture *half_heart = sdl_load_texture(HALF_HEART);
    SDL_Texture *empty_heart = sdl_load_texture(EMPTY_HEART);

    bool spacebar_pressed = false;

    sdl_on_key(on_key);
    while(!sdl_is_done(game)) {
        list_t *UIs = scene_get_UI_components(game_get_current_scene(game));
        UI_t *game_screen = list_get(UIs, list_size(UIs) - 1);
        if (strcmp(UI_get_type(game_screen), "START") != 0 && !spacebar_pressed) {
            spacebar_pressed = true;
        }

        scene_t *scene = game_get_current_scene(game);
        double dt = time_since_last_tick();
        if (spacebar_pressed) {
            handle_enemies(game, dt);
        }
        scene_tick(scene, dt);
        seconds += dt;
        sdl_set_camera(vec_subtract(body_get_centroid(game_get_player(game)), (vector_t) {1024 / 2, 512 / 2}));
        body_t *player = game_get_player(game);

        // Player health display (heart) adjustment
        double lost_player_halves = (PLAYER_HEALTH - body_get_stats_info(player).health) / HALF_HEART_HEALTH;
        int full_hearts_lost = floor(lost_player_halves / 2);
        bool half_heart_lost = false;
        if (round(lost_player_halves) > full_hearts_lost * 2) half_heart_lost = true;
        list_t *hearts = get_player_hearts(scene);
        for (size_t i = list_size(hearts) - 1; i > list_size(hearts) - 1 - full_hearts_lost; i--) {
            UI_set_texture(list_get(hearts, i), empty_heart);
        }
        if (half_heart_lost) {
            size_t idx = list_size(hearts) - 1 - full_hearts_lost;
            UI_set_texture(list_get(hearts, idx), half_heart);
        }

        if (body_get_stats_info(player).health <= 0) {
            scene_t *scene_new = scene_reset(game);
            player_info = body_get_stats_info(player);
            player_info.health = PLAYER_HEALTH;
            body_t *player_new = make_demo_sprite(game, 100, 100, "PLAYER", player_info);
            game_set_player(game, player_new);
            game_set_current_scene(game, scene_new);
            scene_add_body(scene_new, player_new);
            list_free(hearts);
            scene_free(scene);
            map_load(game, "assets/levels/map_bigger.map", 20, 20);
            create_tile_collision(game_get_current_scene(game), game_get_player(game));

        }

        sdl_render_game(game);
    }
    game_free(game);
    return 0;
}
