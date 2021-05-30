#include "keyhandler.h"
#include "collision.h"

const double player_velocity = 300;

void body_set_velocity_and_direction(body_t *player, vector_t velocity) {
    body_set_velocity(player, velocity); //pass player first
    body_set_direction(player, vec_unit(body_get_velocity(player)));
}

void player_make_bullet(game_t *game, body_t *player, scene_t *scene, vector_t bullet_dir) {
    if(body_get_shoot_cooldown(player) > 0) return;
    body_t *bullet = make_bullet(game, player, bullet_dir, 3, 400); //TODO: Magic numbers
    scene_add_body(scene, bullet);
    create_tile_collision(scene, bullet);
    for (size_t i = 0; i < scene_bodies(scene); i++) {
        if (body_get_type(scene_get_body(scene, i)) >= ENEMY) {
            create_semi_destructive_collision(scene, scene_get_body(scene, i), scene_get_body(scene, scene_bodies(scene) - 1));
        }
    }
    body_set_shoot_cooldown(player, body_get_stats_info(player).cooldown); // TODO: magic number
}

void set_zero_velocity_direction(body_t *player) {
    if (body_get_velocity(player).x != 0 || body_get_velocity(player).y != 0) {
        body_set_direction(player, vec_unit(body_get_velocity(player)));
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
                velocity.x = -player_velocity;
                body_set_velocity_and_direction(player, velocity);
                break;
            case 'd':
                velocity.x = player_velocity;
                body_set_velocity_and_direction(player, velocity);
                break;
            case 's':
                velocity.y = -player_velocity;
                body_set_velocity_and_direction(player, velocity);
                break;
            case 'w':
                velocity.y = player_velocity;
                body_set_velocity_and_direction(player, velocity);
                break;
            case 'i':
                bullet_dir.y = 1;
                player_make_bullet(game, player, scene, bullet_dir);
                break;
            case 'j':
                bullet_dir.x = -1;
                player_make_bullet(game, player, scene, bullet_dir);
                break;
            case 'k':
                bullet_dir.y = -1;
                player_make_bullet(game, player, scene, bullet_dir);
                break;
            case 'l':
                bullet_dir.x = 1;
                player_make_bullet(game, player, scene, bullet_dir);
                break;
            case 'f': {
                list_t *interactors = game_get_tile_interactors(game);
                for(size_t i = 0; i<list_size(interactors); i++) {
                    tile_interactor_t *interactor = list_get(interactors, i);
                    if(find_collision(interactor->area, body_get_hitbox(player)).collided) {
                        interactor->interaction(game);
                        break;
                    }
                }
                break;
            }
            case ESCAPE: {
                list_t *ui_components = scene_get_UI_components(scene);
                for(size_t i = 0; i<list_size(ui_components); i++) {
                    if(strcmp(UI_get_type(list_get(ui_components, i)), "MURAL") == 0) {
                        list_remove(ui_components, i);
                    }
                }
                game_set_paused(game, !game_is_paused(game));
                break;
            }
        }
    }
    else if (type == KEY_RELEASED) {
        switch (key) {
            case 'a':
                body_set_direction(player, vec_unit(body_get_velocity(player)));
                if (body_get_velocity(player).x == -player_velocity) {
                    velocity.x = 0;
                    body_set_velocity(player, velocity);
                }
                set_zero_velocity_direction(player);
                if (body_get_velocity(player).x == 0 && body_get_velocity(player).y == 0) body_set_sprite(player, game_get_sprite(game, 0));
                break;
            case 'd':
                body_set_direction(player, vec_unit(body_get_velocity(player)));
                if(body_get_velocity(player).x == player_velocity) {
                    velocity.x = 0;
                    body_set_velocity(player, velocity);
                }
                set_zero_velocity_direction(player);
                if (body_get_velocity(player).x == 0 && body_get_velocity(player).y == 0) body_set_sprite(player, game_get_sprite(game, 0));
                break;
            case 's':
                body_set_direction(player, vec_unit(body_get_velocity(player)));
                if(body_get_velocity(player).y == -player_velocity) {
                    velocity.y = 0;
                    body_set_velocity(player, velocity);
                }
                set_zero_velocity_direction(player);
                if (body_get_velocity(player).x == 0 && body_get_velocity(player).y == 0) body_set_sprite(player, game_get_sprite(game, 0));
                break;
            case 'w':
                body_set_direction(player, vec_unit(body_get_velocity(player)));
                if(body_get_velocity(player).y == player_velocity) {
                    velocity.y = 0;
                    body_set_velocity(player, velocity);
                }
                set_zero_velocity_direction(player);
                if (body_get_velocity(player).x == 0 && body_get_velocity(player).y == 0) body_set_sprite(player, game_get_sprite(game, 0));
                break;
        }
    }
}

