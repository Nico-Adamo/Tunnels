#ifndef __GAME_H__
#define __GAME_H__

#include "list.h"
#include "scene.h"
#include "sprite.h"

typedef struct game game_t;

typedef void (*tile_interaction)(game_t *game);

enum tile_interactor_type{PRESS_F, MURAL};

typedef struct tile_interactor {
    rect_t area;
    tile_interaction interaction;
    enum tile_interactor_type type;
} tile_interactor_t;

game_t *game_init(double scale);

void game_free(void *game);

void game_add_tile_info(game_t *game, tile_info_t *tile_info);

tile_interactor_t *tile_interactor_init(rect_t area, tile_interaction interaction, enum tile_interactor_type type);

void game_add_tile_interactor(game_t *game, tile_interactor_t *interactor);

list_t *game_get_tile_interactors(game_t *game);

void game_reset_tile_interactors(game_t *game);

void game_add_sprite(game_t *game, sprite_t *sprite);

scene_t *game_get_current_scene(game_t *game);

void game_set_current_scene(game_t *game, scene_t *scene);

double game_get_scale(game_t *game);

body_t *game_get_player(game_t *game);

void game_set_player(game_t *game, body_t *player);

tile_info_t *game_get_tile_info(game_t *game, size_t tile_id);

sprite_t *game_get_sprite(game_t *game, size_t sprite_id);

void game_register_sprites(game_t *game);

void game_reset_dungeon(game_t *game);

void game_add_room(game_t *game, const char *path);

list_t *game_get_dungeon(game_t *game);

size_t game_get_room(game_t *game);

void game_next_room(game_t *game);

void game_set_room(game_t *game, int room);

size_t game_get_level(game_t *game);

void game_set_level(game_t *game, size_t level);

bool game_is_paused(game_t *game);

void game_set_paused(game_t *game, bool paused);

size_t game_get_dungeon_size(game_t *game);

#endif // #ifndef __GAME_H__
