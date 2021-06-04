#include "game.h"
#include "enemy.h"
#include "scene.h"
#include "list.h"
#include "vector.h"
#include "body.h"
#include "map.h"
#include <math.h>
#include <stdlib.h>
#include "forces.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "collision.h"
#include "keyhandler.h"
#include "user_interface.h"
#include "sprite.h"
#include "sdl_wrapper.h"
#include "level.h"

const char *PRESS_F_PATH = "assets/ui/pressF.png";
const double level_up_buffs[5] = {/*HEALTH*/ 10, /*ATTACK*/ 5, /*COOLDOWN*/ .8, /*SPEED*/ 50, /*INVULNERABILITY*/ 1.5};
const double INIT_LEVEL_EXP = 100;
const double LEVEL_EXP_FACTOR = 1.5;


void display_hearts(game_t *game, scene_t *scene, body_t *player) {
    double length = 60;
    sprite_t *half_heart = game_get_sprite(game, HALF_HEART_ID);
    sprite_t *empty_heart = game_get_sprite(game, EMPTY_HEART_ID);
    sprite_t *full_heart = game_get_sprite(game, FULL_HEART_ID);
    double curr_health = body_get_stats_info(player).health;
    list_t *hearts = get_player_hearts(scene);
    size_t num_hearts = list_size(hearts);
    if (num_hearts > 0) {
        double max_health = num_hearts * HALF_HEART_HEALTH * 2;
        if (body_get_stats_info(player).health > max_health) {
            UI_t *heart = make_heart(HEART_PADDING + 32 * (num_hearts) + length, MAX_HEIGHT - 32 - HEART_PADDING, game_get_sprite(game, FULL_HEART_ID), "PLAYER_HEART");
            scene_add_UI_component(scene, heart);
            list_free(hearts);
            hearts = get_player_hearts(scene);
            max_health += HALF_HEART_HEALTH * 2;
        }
        double lost_player_halves = (max_health - body_get_stats_info(player).health) / HALF_HEART_HEALTH;
        int full_hearts_lost = floor(lost_player_halves / 2);
        bool half_heart_lost = false;
        if (lost_player_halves - full_hearts_lost * 2 > .5) half_heart_lost = true;
        for (size_t i = 0; i < list_size(hearts); i++) {
            UI_set_sprite(list_get(hearts, i), full_heart);
        }
        for (size_t i = list_size(hearts) - 1; i > list_size(hearts) - full_hearts_lost - 1; i--) {
            UI_set_sprite(list_get(hearts, i), empty_heart);
        }
        if (half_heart_lost) {
            size_t idx = list_size(hearts) - 1 - full_hearts_lost;
            if(idx < list_size(hearts)) {
                UI_set_sprite(list_get(hearts, idx), half_heart);
            }
        }
    }
    list_free(hearts);
}

void display_experience(game_t *game, scene_t *scene, body_t *player) {
    stats_info_t player_stats = body_get_stats_info(player);
    sprite_t *coin_filled = game_get_sprite(game, COIN_FILLED_ID);
    sprite_t *coin_empty = game_get_sprite(game, COIN_EMPTY_ID);
    int level_up_exp = round(INIT_LEVEL_EXP * pow(LEVEL_EXP_FACTOR, player_stats.level - 1));
    double curr_exp = player_stats.experience;
    list_t *coins = get_player_coins(scene);
    size_t num_coins = list_size(coins);
    if (num_coins > 0) {
        double coin_exp = level_up_exp / 10;
        size_t filled_coin_num = floor(curr_exp / coin_exp);
        for (size_t i = 0; i < filled_coin_num; i++) {
            if(i < list_size(coins)) UI_set_sprite(list_get(coins, i), coin_filled);
        }
        for (size_t i = filled_coin_num; i < num_coins; i++) {
            if(i < list_size(coins)) UI_set_sprite(list_get(coins, i), coin_empty);
        }
    }
    list_free(coins);
}

int main(int arg_c, char *arg_v[]) {
    srand(time(0));
    vector_t bottom_left = {.x = 0, .y = 0};
    vector_t top_right = {.x = MAX_WIDTH, .y = MAX_HEIGHT};
    sdl_init(bottom_left, top_right);

    stats_info_t player_info = {
        .experience = 0,
        .attack = 5,
        .health = PLAYER_HEALTH,
        .cooldown = 0.3,
        .invulnerability_timer = .6,
        .level = 1,
        .speed = 300,
        .atk_type = RADIAL_SHOOTER
    };

    game_t *game = game_init(4);                                                                                                                                                                               game_init(4);

    map_register_tiles(game);
    map_register_collider_tiles();

    game_register_sprites(game);

    body_t *player = make_player(game, 100, 100, PLAYER, player_info);
    scene_t *scene = make_title(game);
    scene_add_body(scene, player);

    game_set_player(game, player);
    game_set_current_scene(game, scene);

    shuffle_levels();
    make_level(game, 0);

    double seconds = 0;

    bool spacebar_pressed = false;
    bool entered_door = false;
    double powerup_timer = 5;

    size_t cur_room;

    sdl_on_key(on_key);

    while(!sdl_is_done(game)) {
        handle_movement_shooting(game);
        if (!scene_is_menu(game_get_current_scene(game)) && !spacebar_pressed) {
            spacebar_pressed = true;
            make_room(game);
            ui_text_t *level_text = ui_text_init("Level 1", (vector_t) {5, MAX_HEIGHT - 105}, INFINITY, EXP_TEXT);
            scene_add_UI_text(game_get_current_scene(game), level_text);
            cur_room = game_get_room(game);
        }

        scene_t *scene = game_get_current_scene(game);
        double dt = time_since_last_tick();
        if(!game_is_paused(game)) {
            seconds += dt;
            if(seconds > 0.5) {
                handle_enemies(game, dt);
                seconds = 0;
            }
            scene_tick(scene, dt);
            sdl_set_camera(vec_subtract(body_get_centroid(game_get_player(game)), (vector_t) {1024 / 2, 512 / 2}));
            body_t *player = game_get_player(game);

            // Player health display (heart) adjustment
            display_hearts(game, scene, player);

            // Player experience display adjustment
            display_experience(game, scene, player);

            // Interaction UI display
            bool in_flag = entered_door;
            entered_door = UI_handle_door_interaction(game, entered_door);
            if(entered_door == false && in_flag == false) { // Leaving
                list_t *UIs = scene_get_UI_components(scene);
                for (size_t i = 0; i < list_size(UIs); i++) {
                    if (strcmp(UI_get_type(list_get(UIs, i)), "PRESS_F") == 0 || strcmp(UI_get_type(list_get(UIs, i)), "MURAL") == 0) {
                        list_remove(UIs, i);
                    }
                }
            }

            // Interaction with Level Stuff
            list_t *UI_comps = scene_get_UI_components(scene);
            if (list_size(UI_comps) != 0) {
                for (size_t i = 0; i < list_size(UI_comps); i++) {
                    if (strcmp(UI_get_type(list_get(UI_comps, i)), "LEVEL_UP") == 0) {
                        if (powerup_timer > 0) powerup_timer -= dt;
                        else {
                            list_remove (UI_comps, i);
                            powerup_timer = 5;
                        }
                        break;
                    }
                }
            }


            // Levelling up + Text Display
            stats_info_t player_stats = body_get_stats_info(player);
            char level[100];
            sprintf(level, "Level %d", player_stats.level);
            int level_up_exp = round(INIT_LEVEL_EXP * pow(LEVEL_EXP_FACTOR, player_stats.level - 1));
            if (player_stats.experience >= level_up_exp) {
                ui_text_t *level_text;
                list_t *texts = scene_get_UI_texts(scene);
                for (size_t i = 0; i < list_size(texts); i++) {
                    if (ui_text_get_type(list_get(texts, i)) == EXP_TEXT) {
                        level_text = list_get(texts, i);
                        break;
                    }
                }
                player_stats.experience -= level_up_exp;
                player_stats.level++;
                char level_cur[100];
                sprintf(level_cur, "Level %d", player_stats.level);
                ui_text_set_message(level_text, level_cur);
                int buff = rand() % 5; // TODO: Magic Numbers?
                sprite_t *powerup_sprite;
                switch (buff) {
                    case 0:
                        player_stats.health += level_up_buffs[0];
                        powerup_sprite = game_get_sprite(game, 53);
                        break;
                    case 1:
                        player_stats.attack += level_up_buffs[1];
                        powerup_sprite = game_get_sprite(game, 54);
                        break;
                    case 2:
                        player_stats.cooldown *= level_up_buffs[2];
                        powerup_sprite = game_get_sprite(game, 55);
                        break;
                    case 3:
                        player_stats.speed += level_up_buffs[3];
                        powerup_sprite = game_get_sprite(game, 56);
                        break;
                    case 4:
                        player_stats.invulnerability_timer *= level_up_buffs[4];
                        powerup_sprite = game_get_sprite(game, 57);
                        break;
                }
                rect_t player_hitbox = body_get_hitbox(player);
                double buffer_dist = 40;
                SDL_Rect sprite_size = sprite_get_shape(powerup_sprite, 1);
                UI_t *component = UI_init(powerup_sprite, (rect_t) {512 - sprite_size.w/4, 256 + player_hitbox.h / 2 + buffer_dist, sprite_size.w/2, sprite_size.h/2}, "LEVEL_UP", .5);
                scene_add_UI_component(scene, component);
                body_set_stats_info(player, player_stats);
            }
            if (cur_room != game_get_room(game) && spacebar_pressed) {
                scene_add_UI_text(scene, ui_text_init(level, (vector_t) {5, MAX_HEIGHT - 105}, INFINITY, EXP_TEXT));
                cur_room = game_get_room(game);
            }

            if (body_get_stats_info(player).health <= 0) {
                game_set_room(game, 0);
                make_room(game);
                stats_info_t player_info = body_get_stats_info(game_get_player(game));
                player_info.health = list_size(get_player_hearts(game_get_current_scene(game))) * 10;
                body_set_stats_info(game_get_player(game), player_info);
                char level[100];
                sprintf(level, "Level %d", player_info.level);
                scene_add_UI_text(game_get_current_scene(game), ui_text_init(level, (vector_t) {5, MAX_HEIGHT - 105}, INFINITY, EXP_TEXT));
                cur_room = game_get_room(game);
            }
        }
        sdl_render_game(game);
    }
    game_free(game);
    return 0;
}
