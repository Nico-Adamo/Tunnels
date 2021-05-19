#ifndef __MAP_H__
#define __MAP_H__

#include "game.h"

void map_load(game_t *game, const char *path, size_t x_tiles, size_t y_tiles);

void map_register_tiles(game_t *game);

#endif // #ifndef __MAP_H__
