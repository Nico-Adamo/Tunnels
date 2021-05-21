#include "game.h"

const size_t NUM_TILES = 40; //TODO: update
const size_t NUM_SPRITES = 40;

typedef struct game {
    list_t *tile_infos;
    list_t *sprites;
    scene_t *current_scene;
    double scale;
    body_t *player;
} game_t;

game_t *game_init(double scale) {
    game_t *game = malloc(sizeof(game_t));

    game->current_scene = NULL;
    game->tile_infos = list_init(NUM_TILES, tile_info_free);
    game->sprites = list_init(NUM_SPRITES, tile_info_free);
    game->scale = scale;
    game->player = NULL;

    return game;
}

void game_free(void *game) {
    game_t *game_c = (game_t *) game;
    list_free(game_c->tile_infos);
    scene_free(game_c->current_scene);
    free(game_c);
}

void game_add_tile_info(game_t *game, tile_info_t *tile_info) {
    list_add(game->tile_infos, tile_info);
}

void game_add_sprite(game_t *game, sprite_t *sprite) {
    list_add(game->sprites, sprite);
}

scene_t *game_get_current_scene(game_t *game) {
    return game->current_scene;
}

void game_set_current_scene(game_t *game, scene_t *scene) {
    game->current_scene = scene;
}

double game_get_scale(game_t *game) {
    return game->scale;
}

body_t *game_get_player(game_t *game) {
    return game->player;
}

void game_set_player(game_t *game, body_t *player) {
    game->player = player;
}

tile_info_t *game_get_tile_info(game_t *game, size_t tile_id) {
    return list_get(game->tile_infos, tile_id);
}

sprite_t *game_get_sprite(game_t *game, size_t sprite_id) {
    return list_get(game->sprites, sprite_id);
}
