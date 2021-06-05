#ifndef __MAP_H__
#define __MAP_H__

#include "game.h"
#include "level.h"

/**
 * Loads a specified map.
 *
 * @param game a pointer to a game
 * @param path the file path for the map to be loaded
 */
void map_load(game_t *game, const char *path);

/**
 * Registers all tiles for a given game.
 *
 * @param game a pointer to a game
 */
void map_register_tiles(game_t *game);

/**
 * Registers all collider tiles for a given game.
 *
 * @param game a pointer to a game
 */
void map_register_collider_tiles();

#endif // #ifndef __MAP_H__
