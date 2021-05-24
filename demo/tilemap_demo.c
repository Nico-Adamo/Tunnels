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
#include "sprite.h"
#include "sdl_wrapper.h"
#include "level.h"

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

    body_t *player = make_player(game, 100, 100, "PLAYER", player_info);
    scene_t *scene = make_title(game);
    scene_add_body(scene, player);

    game_set_player(game, player);
    game_set_current_scene(game, scene);

    double seconds = 0;
    SDL_Texture *half_heart = sdl_load_texture(HALF_HEART);
    SDL_Texture *empty_heart = sdl_load_texture(EMPTY_HEART);

    bool spacebar_pressed = false;

    sdl_on_key(on_key);
    while(!sdl_is_done(game)) {
        if (!scene_is_menu(game_get_current_scene(game)) && !spacebar_pressed) {
            spacebar_pressed = true;
            make_level(game);
            scene_free(scene);
        }

        scene_t *scene = game_get_current_scene(game);
        double dt = time_since_last_tick();
        handle_enemies(game, dt);
        scene_tick(scene, dt);
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
            body_t *player_new = make_player(game, 100, 100, "PLAYER", player_info);
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
