#include "game.h"

const size_t num_tiles = 40; //TODO: update

typedef struct game {
    list_t *tile_infos;
    scene_t *current_scene;
    double scale;
    body_t *player;
} game_t;

game_t *game_init(scene_t *initial_scene, body_t *player, double scale) {
    game_t *game = malloc(sizeof(game_t));

    game->current_scene = initial_scene;
    game->tile_infos = list_init(num_tiles, tile_info_free);
    game->scale = scale;
    game->player = player;

    return game;
}

void game_free(void *game) {
    game_t *game_c = (game_t *) game;
    list_free(game_c->tile_infos);
    scene_free(game_c->current_scene);
    free(game_c->player);
    free(game_c);
}

void game_add_tile_info(game_t *game, tile_info_t *tile_info) {
    list_add(game->tile_infos, tile_info);
}

scene_t *game_get_current_scene(game_t *game) {
    return game->current_scene;
}

double game_get_scale(game_t *game) {
    return game->scale;
}

body_t *game_get_player(game_t *game) {
    return game->player;
}

tile_info_t *game_get_tile_info(game_t *game, size_t tile_id) {
    return list_get(game->tile_infos, tile_id);
}
