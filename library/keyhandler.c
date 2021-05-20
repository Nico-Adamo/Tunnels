#include "keyhandler.h"

const double player_velocity = 300;

void body_set_velocity_and_direction(body_t *player, vector_t velocity) {
    body_set_velocity(player, velocity); //pass player first
    body_set_direction(player, vec_unit(body_get_velocity(player)));
}

void player_make_bullet(body_t *player, scene_t *scene, vector_t bullet_dir) {
    body_t *bullet = make_demo_bullet(player, bullet_dir);
    scene_add_body(scene, bullet);
    create_tile_collision(scene, bullet);
    for (size_t i = 0; i < scene_bodies(scene); i++) {
        if (strcmp(body_get_type(scene_get_body(scene, i)), "ENEMY") == 0) {
            create_semi_destructive_collision(scene, scene_get_body(scene, i), scene_get_body(scene, scene_bodies(scene) - 1));
        }
    }
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
                player_make_bullet(player, scene, bullet_dir);
                break;
            case 'j':
                bullet_dir.x = -1;
                player_make_bullet(player, scene, bullet_dir);
                break;
            case 'k':
                bullet_dir.y = -1;
                player_make_bullet(player, scene, bullet_dir);
                break;
            case 'l':
                bullet_dir.x = 1;
                player_make_bullet(player, scene, bullet_dir);
                break;

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
                break;
            case 'd':
                body_set_direction(player, vec_unit(body_get_velocity(player)));
                if(body_get_velocity(player).x == player_velocity) {
                    velocity.x = 0;
                    body_set_velocity(player, velocity);
                }
                set_zero_velocity_direction(player);
                break;
            case 's':
                body_set_direction(player, vec_unit(body_get_velocity(player)));
                if(body_get_velocity(player).y == -player_velocity) {
                    velocity.y = 0;
                    body_set_velocity(player, velocity);
                }
                set_zero_velocity_direction(player);
                break;
            case 'w':
                body_set_direction(player, vec_unit(body_get_velocity(player)));
                if(body_get_velocity(player).y == player_velocity) {
                    velocity.y = 0;
                    body_set_velocity(player, velocity);
                }
                set_zero_velocity_direction(player);
                break;
        }
    }
}

