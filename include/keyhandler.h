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

void on_key(char key, key_event_type_t type, double held_time, game_t *game);

#endif // #ifndef __KEYHANDLER_H__