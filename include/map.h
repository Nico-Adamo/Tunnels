#ifndef __MAP_H__
#define __MAP_H__

#include "game.h"
#include "level.h"

void map_load(game_t *game, const char *path);

void map_register_tiles(game_t *game);

void map_register_collider_tiles();

#endif // #ifndef __MAP_H__
