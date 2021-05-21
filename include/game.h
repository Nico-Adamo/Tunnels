#ifndef __GAME_H__
#define __GAME_H__

#include "list.h"
#include "scene.h"
#include "sprite.h"

typedef struct game game_t;

game_t *game_init(double scale);

void game_free(void *game);

void game_add_tile_info(game_t *game, tile_info_t *tile_info);

void game_add_sprite(game_t *game, sprite_t *sprite);

scene_t *game_get_current_scene(game_t *game);

void game_set_current_scene(game_t *game, scene_t *scene);

double game_get_scale(game_t *game);

body_t *game_get_player(game_t *game);

void game_set_player(game_t *game, body_t *player);

tile_info_t *game_get_tile_info(game_t *game, size_t tile_id);

sprite_t *game_get_sprite(game_t *game, size_t sprite_id);

void game_register_sprites(game_t *game);

#endif // #ifndef __GAME_H__
