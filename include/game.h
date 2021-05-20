#ifndef __GAME_H__
#define __GAME_H__

#include "list.h"
#include "scene.h"

typedef struct game game_t;

game_t *game_init(scene_t *initial_scene, body_t *player, double scale);

void game_free(void *game);

void game_add_tile_info(game_t *game, tile_info_t *tile_info);

scene_t *game_get_current_scene(game_t *game);

void game_set_current_scene(game_t *game, scene_t *scene);

double game_get_scale(game_t *game);

body_t *game_get_player(game_t *game);

void game_set_player(game_t *game, body_t *player);

tile_info_t *game_get_tile_info(game_t *game, size_t tile_id);

#endif // #ifndef __GAME_H__
