#include "keyhandler.h"
#include "collision.h"
#include "level.h"

const int PAUSE_ID = 63;

const double player_velocity = 300;
const double player_bullet_velocity = 400;

bool key_pressed[8] = {false, false, false, false, false, false, false, false};

void body_set_velocity_and_direction(body_t *player, vector_t velocity) {
    body_set_velocity(player, velocity); //pass player first
    body_set_direction(player, vec_unit(body_get_velocity(player)));
}

void player_make_bullet(game_t *game, body_t *player, scene_t *scene, vector_t bullet_dir) {
    if(body_get_shoot_cooldown(player) > 0) return;
    body_t *bullet = make_bullet(game, player, bullet_dir, 3, player_bullet_velocity);
    scene_add_body(scene, bullet);
    create_tile_collision(scene, bullet);
    for (size_t i = 0; i < scene_bodies(scene); i++) {
        if (body_get_type(scene_get_body(scene, i)) >= ENEMY) {
            create_semi_destructive_collision(scene, scene_get_body(scene, i), scene_get_body(scene, scene_bodies(scene) - 1));
        }
    }
    body_set_shoot_cooldown(player, body_get_stats_info(player).cooldown);
}

void set_zero_velocity_direction(body_t *player) {
    if (body_get_velocity(player).x != 0 || body_get_velocity(player).y != 0) {
        body_set_direction(player, vec_unit(body_get_velocity(player)));
    }
}

void handle_movement_shooting(game_t *game) {
    scene_t *scene = game_get_current_scene(game);
    body_t *player = game_get_player(game);
    vector_t velocity = body_get_velocity(player);
    vector_t bullet_dir = VEC_ZERO;
    if(key_pressed[0] == true) { // "W" held
        velocity.y = player_velocity;
        body_set_velocity_and_direction(player, velocity);
    } if(key_pressed[1] == true) { // "A"
        velocity.x = -player_velocity;
        body_set_velocity_and_direction(player, velocity);
    } if(key_pressed[2] == true) { // "S"
        velocity.y = -player_velocity;
        body_set_velocity_and_direction(player, velocity);
    } if(key_pressed[3] == true) { // "D"
        velocity.x = player_velocity;
        body_set_velocity_and_direction(player, velocity);
    } if(key_pressed[4] == true) { // "I"
        bullet_dir.y = 1;
        player_make_bullet(game, player, scene, bullet_dir);
    } if(key_pressed[5] == true) { // "J"
        bullet_dir.x = -1;
        player_make_bullet(game, player, scene, bullet_dir);
    } if(key_pressed[6] == true) { // "K"
        bullet_dir.y = -1;
        player_make_bullet(game, player, scene, bullet_dir);
    } if(key_pressed[7] == true) { // "L"
        bullet_dir.x = 1;
        player_make_bullet(game, player, scene, bullet_dir);
    }
}

void on_key(char key, key_event_type_t type, double held_time, game_t *game) {
    scene_t *scene = game_get_current_scene(game);
    body_t *player = game_get_player(game);
    vector_t velocity = body_get_velocity(player);
    vector_t bullet_dir = VEC_ZERO;
    // body_t *bullet;
    if (type == KEY_PRESSED) {
        switch (key) {
            case ' ':
                if (scene_is_menu(game_get_current_scene(game))) {
                    scene_set_is_menu(game_get_current_scene(game), false);
                }
                break;
            case 'a':
                key_pressed[1] = true;
                break;
            case 'd':
                key_pressed[3] = true;
                break;
            case 's':
                key_pressed[2] = true;
                break;
            case 'w':
                key_pressed[0] = true;
                break;
            case 'i':
                key_pressed[4] = true;
                break;
            case 'j':
                key_pressed[5] = true;
                break;
            case 'k':
                key_pressed[6] = true;
                break;
            case 'l':
                key_pressed[7] = true;
                break;
            case 'f': {
                list_t *interactors = game_get_tile_interactors(game);
                int i = 0;
                while(i < list_size(interactors)) {
                    tile_interactor_t *interactor = list_get(interactors, i);
                    if(find_collision(interactor->area, body_get_hitbox(player)).collided) {
                        interactor->interaction(game);
                        if(interactor->type == MURAL) {
                            list_remove(interactors, i);
                            i--;
                        }
                        break;
                    }
                    i++;
                }
                break;
            }
            case ESCAPE: {
                list_t *ui_components = scene_get_UI_components(scene);
                for(size_t i = 0; i < list_size(ui_components); i++) {
                    if(strcmp(UI_get_type(list_get(ui_components, i)), "MURAL") == 0) {
                        list_remove(ui_components, i);
                        random_room_music();
                        ui_text_t *text = ui_text_init("Health restored", (vector_t) {675, HEART_PADDING}, 3, OBJECTIVE_TEXT);
                        scene_add_UI_text(game_get_current_scene(game), text);
                    }
                }
                game_set_paused(game, !game_is_paused(game));
                if (game_is_paused(game)) {
                    Mix_PauseMusic();
                    scene_add_UI_component(scene, UI_init(game_get_sprite(game, PAUSE_ID), (rect_t) {0, 0, 1024, 512}, "PAUSE", 1));
                } else {
                    Mix_ResumeMusic();
                    for(size_t i = 0; i < list_size(ui_components); i++) {
                        if(strcmp(UI_get_type(list_get(ui_components, i)), "PAUSE") == 0) {
                            list_remove(ui_components, i);
                        }
                    }
                }
                break;
            }
        }
    }
    else if (type == KEY_RELEASED) {
        switch (key) {
            case 'a':
                key_pressed[1] = false;
                body_set_direction(player, vec_unit(body_get_velocity(player)));
                if (body_get_velocity(player).x == -player_velocity) {
                    velocity.x = 0;
                    body_set_velocity(player, velocity);
                }
                set_zero_velocity_direction(player);
                if (body_get_velocity(player).x == 0 && body_get_velocity(player).y == 0) body_set_sprite(player, game_get_sprite(game, 0));
                break;
            case 'd':
                key_pressed[3] = false;
                body_set_direction(player, vec_unit(body_get_velocity(player)));
                if(body_get_velocity(player).x == player_velocity) {
                    velocity.x = 0;
                    body_set_velocity(player, velocity);
                }
                set_zero_velocity_direction(player);
                if (body_get_velocity(player).x == 0 && body_get_velocity(player).y == 0) body_set_sprite(player, game_get_sprite(game, 0));
                break;
            case 's':
                key_pressed[2] = false;
                body_set_direction(player, vec_unit(body_get_velocity(player)));
                if(body_get_velocity(player).y == -player_velocity) {
                    velocity.y = 0;
                    body_set_velocity(player, velocity);
                }
                set_zero_velocity_direction(player);
                if (body_get_velocity(player).x == 0 && body_get_velocity(player).y == 0) body_set_sprite(player, game_get_sprite(game, 0));
                break;
            case 'w':
                key_pressed[0] = false;
                body_set_direction(player, vec_unit(body_get_velocity(player)));
                if(body_get_velocity(player).y == player_velocity) {
                    velocity.y = 0;
                    body_set_velocity(player, velocity);
                }
                set_zero_velocity_direction(player);
                if (body_get_velocity(player).x == 0 && body_get_velocity(player).y == 0) body_set_sprite(player, game_get_sprite(game, 0));
                break;
            case 'i':
                key_pressed[4] = false;
                break;
            case 'j':
                key_pressed[5] = false;
                break;
            case 'k':
                key_pressed[6] = false;
                break;
            case 'l':
                key_pressed[7] = false;
                break;
        }
    }
}

