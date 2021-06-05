#ifndef __KEYHANDLER_H__
#define __KEYHANDLER_H__

#include <stdbool.h>
#include <stdlib.h>
#include "sdl_wrapper.h"
#include "body.h"
#include "game.h"
#include "forces.h"
#include "collision.h"
#include "enemy.h"
#include "projectile.h"
#include "ui_component.h"

/**
 * Handles key presses.
 *
 * @param key the pressed key
 * @param type the type of key event
 * @param held_time how long the key has been held
 * @param game a pointer to a game
 */
void on_key(char key, key_event_type_t type, double held_time, game_t *game);

/**
 * Handles movement shooting.
 *
 * @param game a pointer to a game
 */
void handle_movement_shooting(game_t *game);
#endif // #ifndef __KEYHANDLER_H__
