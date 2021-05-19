#ifndef __GAME_H__
#define __GAME_H__

#include "list.h"
#include "scene.h"

typedef struct game {
    list_t *tile_infos;
    scene_t *current_scene;
    double scale;
    vector_t camera; // x,y of bottom left of screen
} game_t;

game_t *game_init(scene_t *initial_scene, double scale);

void game_add_tile_info(game_t *game, tile_info_t *tile_info);

scene_t *game_get_current_scene(game_t *game);

double game_get_scale(game_t *game);

tile_info_t *game_get_tile_info(game_t *game, size_t tile_id);

// TODO: Game free

#endif // #ifndef __GAME_H__
