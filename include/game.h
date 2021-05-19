#ifndef __GAME_H__
#define __GAME_H__

#include "list.h"
#include "scene.h"
#include "tile.h"

typedef struct game {
    list_t *tile_infos;
    scene_t *current_scene;
} game_t; 

game_t *game_init(scene_t *initial_scene);

void game_add_tile_info(game_t *game);

scene_t *game_get_current_scene(game_t *game);

tile_info_t *game_get_tile_info(game_t *game, size_t tile_id);

// TODO: Game free

#endif // #ifndef __GAME_H__